#include "Model.h"
#include <fstream>
#include <sstream>
#pragma warning(push, 0)
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#pragma warning(pop)
#include <SpImGui.h>

Model::Model() {
}

Matrix AiMatToSpMat(aiMatrix4x4 m)
{
	return Matrix(
		m.a1, m.a2, m.a3, m.a4,
		m.b1, m.b2, m.b3, m.b4,
		m.c1, m.c2, m.c3, m.c4,
		m.d1, m.d2, m.d3, m.d4
	);
}

Model::Model(const std::string& modelName)
{
	std::string path = "Assets/Models/" + modelName + "/";
	std::string objfile = modelName + ".obj";

	filePath = path + objfile;

	//std::vector<Vertex> vertices;

	std::vector<UINT> indices;

	std::ifstream file;
	file.open(path + objfile);
	if (file.fail()) {
		assert(0);
	}

	std::string line;
	std::vector<Float3> posList;
	std::vector<Vec3> normalList;
	std::vector<Float2> tcList;
	while (getline(file, line)) {
		std::istringstream lineStream(line);

		std::string tag;
		getline(lineStream, tag, ' ');

		if (tag == "v")
		{
			Float3 position{};
			lineStream >> position.x;
			lineStream >> position.y;
			lineStream >> position.z;

			posList.emplace_back(position);
		}

		if (tag == "vt")
		{
			Float2 texcoord{};
			lineStream >> texcoord.x;
			lineStream >> texcoord.y;

			texcoord.y = 1.0f - texcoord.y;
			tcList.emplace_back(texcoord);
		}

		if (tag == "vn")
		{
			Vec3 normal{};

			lineStream >> normal.x;
			lineStream >> normal.y;
			lineStream >> normal.z;

			normalList.emplace_back(normal);
		}

		if (tag == "f")
		{
			std::string indexString;
			while (getline(lineStream, indexString, ' '))
			{
				std::istringstream indexStream(indexString);
				UINT indexPosition;
				indexStream >> indexPosition;

				indexStream.seekg(1, std::ios_base::cur);
				UINT indexTexcoord;
				indexStream >> indexTexcoord;

				indexStream.seekg(1, std::ios_base::cur);
				UINT indexNormal;
				indexStream >> indexNormal;

				Vertex vertex{};
				vertex.pos = posList[indexPosition - 1];
				vertex.normal = normalList[indexNormal - 1];
				vertex.uv = tcList[indexTexcoord - 1];
				vertex.bIndices[0] = 0;
				vertex.bWeights = { 1.f, 0.f, 0.f, 0.f };

				vertices.emplace_back(vertex);

				indices.emplace_back((UINT)indices.size());
			}
		}

		if (tag == "mtllib")
		{
			std::string filename;
			lineStream >> filename;

			LoadMaterial(path, filename);
		}

	}
	file.close();

	bMatrixCB.contents->bMatrix[0] = Matrix::Identity();

	UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * vertices.size());

	////頂点バッファの設定
	D3D12_HEAP_PROPERTIES heapprop{};
	heapprop.Type = D3D12_HEAP_TYPE_UPLOAD;

	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = sizeVB;
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.SampleDesc.Count = 1;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	GetSpDX()->dev->CreateCommittedResource(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff)
	);

	vertBuff->SetName(L"VERT BUFF");

	// GPU上のバッファに対応した仮想メモリを取得
	Vertex* vertMap = nullptr;
	vertBuff->Map(0, nullptr, (void**)&vertMap);

	// 全頂点に対して
	for (uint32_t i = 0; i < vertices.size(); i++)
	{
		vertMap[i] = vertices[i];   // 座標をコピー
	}

	// マップを解除
	vertBuff->Unmap(0, nullptr);

	// 頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(Vertex);

	//頂点インデックスバッファの生成
	UINT sizeIB = static_cast<UINT>(sizeof(UINT) * indices.size());

	resdesc.Width = sizeIB;

	GetSpDX()->dev->CreateCommittedResource(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff)
	);
	indexBuff->SetName(L"INDEX BUFF");

	// GPU上のバッファに対応した仮想メモリを取得
	uint32_t* indexMap = nullptr;
	indexBuff->Map(0, nullptr, (void**)&indexMap);

	// 全indexに対して
	for (int32_t i = 0; i < indices.size(); i++)
	{
		indexMap[i] = indices[i];   // 座標をコピー
	}

	// マップを解除
	indexBuff->Unmap(0, nullptr);

	// 頂点インデックスバッファビューの作成
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R32_UINT;
	ibView.SizeInBytes = sizeIB;
}

Model::Model(const std::string& modelFilePath, bool useSmoothShading)
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(modelFilePath,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType |
		aiProcess_ConvertToLeftHanded |
		(useSmoothShading ? aiProcess_GenSmoothNormals : aiProcess_GenNormals) |
		//aiProcess_GenSmoothNormals |
		aiProcess_FixInfacingNormals
	);

	if (scene == nullptr) {
		OutputDebugStringA(importer.GetErrorString());
		return;
	}

	std::vector<Float3> posList;
	std::vector<Vec3> normalList;
	std::vector<Float4> bWeightList;
	std::vector<eastl::array<int32_t, 4>> bIndexList;
	std::vector<Float2> tcList;
	std::vector<UINT> indices;
	//std::vector<Vertex> vertices;

	UINT backIndex = 0;

	//アニメーション読み込み
	if (scene->HasAnimations())
	{
		for (uint32_t a = 0; a < scene->mNumAnimations; a++)
		{
			Animation anim;

			aiAnimation* aiAni = scene->mAnimations[a];

			anim.duration = aiAni->mDuration;
			anim.tickPerSecond = aiAni->mTicksPerSecond;
			anim.name = aiAni->mName.C_Str();

			for (uint32_t c = 0; c < aiAni->mNumChannels; c++)
			{
				Channel channel;
				aiNodeAnim* aiChan = aiAni->mChannels[c];

				channel.name = aiChan->mNodeName.C_Str();

				//Position
				for (uint32_t t = 0; t < aiChan->mNumPositionKeys; t++)
				{
					aiVector3D aiPos = aiChan->mPositionKeys[t].mValue;
					Float3 pos = { aiPos.x, aiPos.y, aiPos.z };
					channel.translations.push_back({ pos, aiChan->mPositionKeys[t].mTime });
				}

				//Rotation
				for (uint32_t t = 0; t < aiChan->mNumRotationKeys; t++)
				{
					aiQuaternion aiRot = aiChan->mRotationKeys[t].mValue;
					Quaternion rot = Quaternion(aiRot.w, Vec3(aiRot.x, aiRot.y, aiRot.z));
					channel.rotations.push_back({ rot, aiChan->mRotationKeys[t].mTime });
				}

				//Scale
				for (uint32_t s = 0; s < aiChan->mNumScalingKeys; s++)
				{
					aiVector3D aiScale = aiChan->mScalingKeys[s].mValue;
					Float3 scale = { aiScale.x, aiScale.y, aiScale.z };
					channel.scales.push_back({ scale, aiChan->mScalingKeys[s].mTime });
				}

				anim.channels.push_back(channel);
			}
			animations.insert(eastl::pair<std::string, Animation>(anim.name, anim));
		}
	}

	//先にBoneのみ列挙して保存
	std::function<void(aiNode*)>fNodeBone = [&](aiNode* cur)
		{
			for (uint32_t i = 0; i < cur->mNumChildren; i++)
			{
				fNodeBone(cur->mChildren[i]);
			}
			//ノードごとのメッシュについて
			for (uint32_t k = 0; k < cur->mNumMeshes; k++) {

				//メッシュごとの処理
				uint32_t i = cur->mMeshes[k];
				aiMesh* mesh = scene->mMeshes[i];

				//ボーンの情報を保存
				if (mesh->HasBones())
				{
					for (int32_t boneIndex = 0; boneIndex < std::min((int32_t)mesh->mNumBones, ModelConsts::MAX_BONES_PER_MODEL); boneIndex++)
					{
						Bone bone;
						aiMatrix4x4 aioffsetmat = mesh->mBones[boneIndex]->mOffsetMatrix;
						aioffsetmat.Transpose();
						Matrix bOffsetMat = Matrix(
							(float)aioffsetmat.a1, (float)aioffsetmat.a2, (float)aioffsetmat.a3, (float)aioffsetmat.a4,
							(float)aioffsetmat.b1, (float)aioffsetmat.b2, (float)aioffsetmat.b3, (float)aioffsetmat.b4,
							(float)aioffsetmat.c1, (float)aioffsetmat.c2, (float)aioffsetmat.c3, (float)aioffsetmat.c4,
							(float)aioffsetmat.d1, (float)aioffsetmat.d2, (float)aioffsetmat.d3, (float)aioffsetmat.d4
						);
						bone.offsetMatrix = bOffsetMat;
						bone.index = boneIndex;

						bones.insert(eastl::pair<std::string, Bone>(std::string(mesh->mBones[boneIndex]->mName.C_Str()), bone));
					}
				}
				else
				{
					Bone bone;
					bone.offsetMatrix = Matrix::Identity();
					bone.index = 0;
					bones.insert(eastl::pair<std::string, Bone>("", bone));
					bMatrixCB.contents->bMatrix[0] = Matrix::Identity();
				}
			}
		};

	fNodeBone(scene->mRootNode);

	//モデル読み込み
	//ノードごとの処理
	std::function<void(aiNode*, Node*)>fNode = [&](aiNode* cur, Node* parent) {
		//ノードごとのボーン情報を取得
		Node node;
		node.parent = parent;

		////このノード用の変換行列
		//std::function<aiMatrix4x4(aiNode*)>calcMat = [&](aiNode* calcn) {
		//	if (calcn->mParent)
		//		return calcMat(calcn->mParent) * calcn->mTransformation;

		//	return calcn->mTransformation;
		//};

		////aiMatrix4x4 wt = calcMat(cur);

		aiMatrix4x4 wt = cur->mTransformation;

		wt.Transpose();

		node.worldTransform = AiMatToSpMat(wt);

		if (!(bones.count(cur->mName.C_Str())))
		{
			Bone b = { 129, Matrix::Identity(), Matrix::Identity() };
			bones.emplace(cur->mName.C_Str(), b);
		}

		node.bone = &bones.at(cur->mName.C_Str());

		node.name = cur->mName.C_Str();

		nodes.emplace(cur->mName.C_Str(), node);

		for (uint32_t i = 0; i < cur->mNumChildren; i++)
		{
			fNode(cur->mChildren[i], &nodes.at(cur->mName.C_Str()));
		}

		//ノードごとのメッシュについて
		for (uint32_t k = 0; k < cur->mNumMeshes; k++) {
			//メッシュごとの処理
			uint32_t i = cur->mMeshes[k];
			aiMesh* mesh = scene->mMeshes[i];

			UINT lastMaxIndex = backIndex;

			for (uint32_t j = 0; j < mesh->mNumVertices; j++) {
				//頂点ごと
				aiVector3D vertex = mesh->mVertices[j];
				//vertex *= wt;
				posList.push_back({ vertex.x, vertex.y, vertex.z });
				backIndex++;

				//法線
				if (mesh->mNormals)
				{
					aiVector3D norm = mesh->mNormals[j];
					norm.Normalize();
					normalList.emplace_back(norm.x, norm.y, norm.z);
				}
				//UV
				if (mesh->HasTextureCoords(0))
				{
					tcList.push_back({ mesh->mTextureCoords[0][j].x,mesh->mTextureCoords[0][j].y });
				}
				//Bone
				if (mesh->HasBones() || mesh->mNumBones)
				{
					struct BoneData {
						int32_t index;
						float weight;
					};

					std::vector<BoneData> bdlist;

					for (size_t m = 0; m < mesh->mNumBones; m++)
					{
						BoneData bd{};

						bd.index = (int32_t)m;

						for (size_t h = 0; h < mesh->mBones[m]->mNumWeights; h++)
						{
							if (mesh->mBones[m]->mWeights[h].mVertexId == j)
							{
								bd.weight = mesh->mBones[m]->mWeights[h].mWeight;

								bdlist.push_back(bd);

								break;
							}
						}

					}

					std::sort(bdlist.begin(), bdlist.end(), [](const auto& lhs, const auto& rhs) {
						return lhs.weight > rhs.weight;
						});

					eastl::array<int32_t, 4> bInd{};
					eastl::array<float, 4> bWeight{};

					for (size_t m = 0; m < 4; m++)
					{
						if (m < bdlist.size())
						{
							bInd[m] = bdlist.at(m).index;
							bWeight[m] = bdlist.at(m).weight;
						}
						else
						{
							bInd[m] = 0;
							bWeight[m] = 0.f;
						}
					}
					bIndexList.push_back({ bInd[0], bInd[1], bInd[2], bInd[3] });
					bWeightList.push_back({ bWeight[0], bWeight[1], bWeight[2], bWeight[3] });
				}
				else
				{
					bIndexList.push_back({ 0, 0, 0, 0 });
					bWeightList.push_back({ 1.f, 0.f, 0.f, 0.f });
				}

				//頂点データ配列に追加
				vertices.emplace_back(Vertex{ posList.back(), normalList.back(), tcList.back(), bIndexList.back(), bWeightList.back() });
			}

			for (uint32_t j = 0; j < mesh->mNumFaces; j++) {
				//ポリゴンごと
				aiFace face = mesh->mFaces[j];
				for (uint32_t l = 0; l < face.mNumIndices; l++) {
					//インデックスごと
					UINT ind = face.mIndices[l];

					indices.emplace_back(ind + lastMaxIndex);
				}
			}
		}
		};

	//ノードごとの処理呼び出し
	fNode(scene->mRootNode, nullptr);

	for (uint32_t i = 0; i < scene->mNumMaterials; i++)
	{
		//マテリアルごと
		material.emplace_back();
		Material* mtr = &material.back();
		aiMaterial* aimtr = scene->mMaterials[i];

		aiColor3D temp;
		aiString tempstr;

		aimtr->Get(AI_MATKEY_NAME, tempstr);
		mtr->name = tempstr.C_Str();

		aimtr->Get(AI_MATKEY_COLOR_AMBIENT, temp);
		if (temp.r || temp.g || temp.b)
		{
			mtr->ambient = { temp.r, temp.g, temp.b };
		}
		else
		{
			mtr->ambient = { 0.7f, 0.7f, 0.7f };
		}

		aimtr->Get(AI_MATKEY_COLOR_SPECULAR, temp);
		mtr->specular = { temp.r, temp.g, temp.b };

		aimtr->Get(AI_MATKEY_COLOR_DIFFUSE, temp);
		mtr->diffuse = { temp.r, temp.g, temp.b };

		//Textrue
		aimtr->GetTexture(aiTextureType_DIFFUSE, 0, &tempstr);
		//TODO:埋め込みテクスチャの場合の処理		
		int32_t pti = (int32_t)modelFilePath.find_last_of("\\");
		int32_t pti2 = (int32_t)modelFilePath.find_last_of("/");
		std::string filedir = modelFilePath.substr(0, std::max(pti, pti2));

		mtr->textureKey = SpTextureManager::LoadTexture(filedir + std::string("/") + std::string(tempstr.C_Str()), std::string("asmptex:") + filedir + std::string(tempstr.C_Str()));
	}

	// スムージング
	Smoothing();

	UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * vertices.size());

	////頂点バッファの設定
	D3D12_HEAP_PROPERTIES heapprop{};
	heapprop.Type = D3D12_HEAP_TYPE_UPLOAD;

	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = sizeVB;
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.SampleDesc.Count = 1;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	GetSpDX()->dev->CreateCommittedResource(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff)
	);

	vertBuff->SetName(L"VERTEX_BUFF_ASMP");

	// GPU上のバッファに対応した仮想メモリを取得
	Vertex* vertMap = nullptr;
	vertBuff->Map(0, nullptr, (void**)&vertMap);

	// 全頂点に対して
	for (int32_t i = 0; i < vertices.size(); i++)
	{
		vertMap[i] = vertices[i];   // 座標をコピー
	}

	// マップを解除
	vertBuff->Unmap(0, nullptr);

	// 頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(Vertex);

	//頂点インデックスバッファの生成
	UINT sizeIB = static_cast<UINT>(sizeof(uint32_t) * indices.size());

	resdesc.Width = sizeIB;

	GetSpDX()->dev->CreateCommittedResource(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff)
	);

	indexBuff->SetName(L"INDEX_BUFF_ASMP");

	// GPU上のバッファに対応した仮想メモリを取得
	uint32_t* indexMap = nullptr;
	indexBuff->Map(0, nullptr, (void**)&indexMap);

	// 全indexに対して
	for (int32_t i = 0; i < indices.size(); i++)
	{
		indexMap[i] = indices[i];   // 座標をコピー
	}

	// マップを解除
	indexBuff->Unmap(0, nullptr);

	// 頂点インデックスバッファビューの作成
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R32_UINT;
	ibView.SizeInBytes = sizeIB;

	filePath = modelFilePath;

	UpdateMaterial();
}

void Model::LoadMaterial(const std::string& path, const std::string& filename)
{
	std::ifstream file;

	file.open(path + filename);

	if (file.fail()) { assert(0); }

	std::string line;

	while (getline(file, line)) {
		std::istringstream lineStream(line);

		std::string tag;
		getline(lineStream, tag, ' ');

		if (tag[0] == '\t') tag.erase(tag.begin());

		if (tag == "newmtl") {
			std::string str;
			lineStream >> str;
			material.emplace_back();
			material.back().name = str;
		}

		if (tag == "Ka") {
			lineStream >> material.back().ambient.x;
			lineStream >> material.back().ambient.y;
			lineStream >> material.back().ambient.z;
		}

		if (tag == "Kd") {
			lineStream >> material.back().diffuse.x;
			lineStream >> material.back().diffuse.y;
			lineStream >> material.back().diffuse.z;
		}

		if (tag == "Ks") {
			lineStream >> material.back().specular.x;
			lineStream >> material.back().specular.y;
			lineStream >> material.back().specular.z;
		}

		if (tag == "map_Kd") {
			std::string texName;
			lineStream >> texName;
			material.back().textureKey = SpTextureManager::LoadTexture(path + texName, texName);
		}
	}

	UpdateMaterial();
}

void Model::UpdateMaterial()
{
	materialCBs.emplace_back();
	materialCBs.back().contents->ambient = material.front().ambient;
	materialCBs.back().contents->diffuse = material.front().diffuse;
	materialCBs.back().contents->specular = material.front().specular;
	materialCBs.back().contents->alpha = material.front().alpha;
}

void Model::Smoothing()
{
	smoothed = vertices;

	for (size_t i = 0; i < vertices.size(); ++i)
	{
		Vec3 sumNormal = { 0.0f, 0.0f, 0.0f };
		int neighborCount = 0;

		for (size_t j = 0; j < vertices.size(); ++j)
		{
			if (i == j) {
				sumNormal += vertices[j].normal;
				neighborCount++;
			}
			else
			{
				float distance = Vec3::Distance(vertices[i].pos, vertices[j].pos);
				if (distance < 0.01f)
				{
					sumNormal += vertices[j].normal;
					neighborCount++;
				}
			}
		}

		if (neighborCount > 0) 
		{
			sumNormal = sumNormal / static_cast<float>(neighborCount);
		}

		smoothed[i].normal = sumNormal.GetNorm();
	}
}

void Model::SetAnim(std::string animKey)
{
	currentAnim = animKey;
}

void Model::UpdateAnim()
{
	//アニメーション時間に関する更新処理
	Animation* anim = nullptr;
	if (animations.count(currentAnim))
	{
		anim = &animations.find(currentAnim)->second;
	}
	else
	{
		anim = &animations.begin()->second;
	}

	animTimer++;
	if ((double)animTimer / 60.0 * anim->tickPerSecond * (double)aniSpeed >= anim->duration)
	{
		animTimer = 0;
	}

	double aniTick = (double)animTimer / 60.0 * anim->tickPerSecond * (double)aniSpeed;

	//Nodeを使って再帰的に処理を行う
	std::function<Matrix(Node*, Channel*, eastl::unordered_map<std::string, Node>&, eastl::unordered_map<std::string, Bone>&)>
		fCalcParentTransform =
		[&aniTick, &fCalcParentTransform, &anim](Node* node, Channel* channel, eastl::unordered_map<std::string, Node>& nodes, eastl::unordered_map<std::string, Bone>& bones)
		{
			Matrix transform;
			Matrix parentTrans;
			Channel* parentChannel = nullptr;

			if (node == nullptr)
			{
				return Matrix::Identity();
			}
			else
			{
				if (node->parent != nullptr)
				{
					for (auto& a : anim->channels)
					{
						if (a.name == node->parent->name)
						{
							parentChannel = &a;
						}
					}
				}

				parentTrans = fCalcParentTransform(node->parent, parentChannel, nodes, bones);
			}

			AScaleData fst{};
			AScaleData scd{};

			if (channel != nullptr)
			{
				if (channel->scales.size())
				{
					fst = channel->scales.front();
					scd = channel->scales.front();
				}

				for (auto itr = channel->scales.begin(); itr != channel->scales.end(); itr++)
				{
					if (itr->time > aniTick)
					{
						scd = *itr;
						if (itr != channel->scales.begin())
						{
							itr--;
						}

						fst = *itr;
						break;
					}
				}

				Vec3 lerpedScale;

				if (scd.time - fst.time)
				{
					lerpedScale = Vec3::Lerp(fst.scale, scd.scale, (float)((aniTick - fst.time) / (scd.time - fst.time)));
				}
				else
				{
					lerpedScale = fst.scale;
				}

				transform = Matrix::Scale(lerpedScale);

				ARotData fstr{};
				ARotData scdr{};

				for (auto itr = channel->rotations.begin(); itr != channel->rotations.end(); itr++)
				{
					if (itr->time > aniTick)
					{
						scdr = *itr;
						if (itr != channel->rotations.begin())
						{
							itr--;
						}
						fstr = *itr;
						break;
					}
				}

				Quaternion slerpedRot;

				if (scdr.time - fstr.time)
				{
					slerpedRot = Quaternion::Slerp(fstr.rot, scdr.rot, (float)((aniTick - fstr.time) / (scdr.time - fstr.time)));
				}
				else
				{
					slerpedRot = fstr.rot;
				}

				Quaternion finr = slerpedRot;

				transform *= finr.GetRotMat();

				ATransData fstt{};
				ATransData scdt{};

				for (auto itr = channel->translations.begin(); itr != channel->translations.end(); itr++)
				{
					if (itr->time > aniTick)
					{
						scdt = *itr;
						if (itr != channel->translations.begin())
						{
							itr--;
						}
						fstt = *itr;
						break;
					}
				}

				Vec3 lerpedTrans;

				if (scdt.time - fstt.time)
				{
					lerpedTrans = Vec3::Lerp(fstt.translation, scdt.translation, (float)((aniTick - fstt.time) / (scdt.time - fstt.time)));
				}
				else
				{
					lerpedTrans = fstt.translation;
				}

				transform *= Matrix::Translation(lerpedTrans);
			}
			else
			{
				transform = node->worldTransform;
			}

			bones.at(node->name).finalMatrix = bones.at(node->name).offsetMatrix * transform * parentTrans /** node->worldTransform*/;

			return transform * parentTrans;
		};

	for (auto& channel : anim->channels)
	{
		fCalcParentTransform(&nodes.at(channel.name), &channel, nodes, bones);
	}

	std::vector<Bone> finalBones;
	for (auto& b : bones)
	{
		finalBones.push_back(b.second);
	}

	std::sort(finalBones.begin(), finalBones.end(), [](const auto& lhs, const auto& rhs) {
		return lhs.index < rhs.index;
		});

	for (uint32_t i = 0; i < ModelConsts::MAX_BONES_PER_MODEL; i++)
	{
		if (i < finalBones.size())
		{
			bMatrixCB.contents->bMatrix[i] = finalBones.at(i).finalMatrix;
		}
		else
		{
			bMatrixCB.contents->bMatrix[i] = Matrix::Identity();
		}
	}
}

void Model::ResetAnimTimer()
{
	animTimer = 0;
}

void Model::MappingVertex()
{
	// GPU上のバッファに対応した仮想メモリを取得
	Vertex* vertMap = nullptr;
	vertBuff->Map(0, nullptr, (void**)&vertMap);

	// 全頂点に対して
	for (int32_t i = 0; i < vertices.size(); i++)
	{
		vertMap[i] = vertices[i];   // 座標をコピー
	}

	// マップを解除
	vertBuff->Unmap(0, nullptr);
}

void Model::MappingSmoothing()
{
	// GPU上のバッファに対応した仮想メモリを取得
	Vertex* vertMap = nullptr;
	vertBuff->Map(0, nullptr, (void**)&vertMap);

	// 全頂点に対して
	for (int32_t i = 0; i < smoothed.size(); i++)
	{
		vertMap[i] = smoothed[i];   // 座標をコピー
	}

	// マップを解除
	vertBuff->Unmap(0, nullptr);
}

void ModelManager::Register(const std::string& modelName, const ModelKey& key)
{
	sModels.Access(
		[&](auto& map) {
			map.emplace(key, modelName).first->second.key = key;
		}
	);

	sPerSceneModels[sCurrentSceneResIndex].push_back(key);
}

void ModelManager::Register(const std::string& modelPath, const ModelKey& key, bool useAssimp)
{
	sModels.Access(
		[&](auto& map) {
			map.emplace(eastl::piecewise_construct,
				eastl::forward_as_tuple(key),
				eastl::forward_as_tuple(modelPath, useAssimp)).first->second.key = key;
		}
	);

	sPerSceneModels[sCurrentSceneResIndex].push_back(key);
}

Model* ModelManager::GetModel(const ModelKey& key)
{
	Model* ret = nullptr;
	sModels.Access(
		[&](auto& map) {
			auto itr = map.find(key);
			if (itr == map.end()) ret = nullptr;
			else ret = &itr->second;
		}
	);

	return ret;
}

void ModelManager::ReleasePerSceneModel()
{
	int32_t lastSceneResIndex = sCurrentSceneResIndex == 0 ? 1 : 0;
	sPerSceneModels[lastSceneResIndex].sort();
	sPerSceneModels[lastSceneResIndex].unique();
	for (auto itr = sPerSceneModels[lastSceneResIndex].begin(); itr != sPerSceneModels[lastSceneResIndex].end(); itr++)
	{
		bool usingInCurrentScene = false;
		for (auto& key : sPerSceneModels[sCurrentSceneResIndex])
		{
			if (key == *itr)
			{
				usingInCurrentScene = true;
			}
		}

		if (!usingInCurrentScene) //今のシーンで使われていないならリリース
		{
			sModels.Access(
				[&](auto& map) {
					map.erase(*itr);
				}
			);
		}
	}

	sPerSceneModels[lastSceneResIndex].clear();
}

void ModelManager::ReleaseAllModels()
{
	sModels.clear();
}

void ModelManager::PreLoadNewScene()
{
	sCurrentSceneResIndex = sCurrentSceneResIndex == 0 ? 1 : 0;
}


exc_unordered_map<ModelKey, Model> ModelManager::sModels;
eastl::list<ModelKey> ModelManager::sPerSceneModels[2];
int32_t ModelManager::sCurrentSceneResIndex = 0;