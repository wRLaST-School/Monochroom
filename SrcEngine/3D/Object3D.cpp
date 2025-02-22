#include "Object3D.h"
#include <SpRootSignature.h>
#include <GPipeline.h>
#include <Light.h>
#include <Camera.h>
#include <SpMath.h>
#include <SpRenderer.h>
#include <SpImGui.h>
#include <Input.h>
#include <DockPanel.h>
#include <RTVManager.h>
#include <GrayScale.h>

float Object3D::dissolveStrength = 1.25f;

Object3D::Object3D()
{
	{ transformCB.contents->mat = Matrix::Identity(); *brightnessCB.contents = { 1.0f, 1.0f, 1.0f, 1.0f }; miscCB.contents->rimColor = { 1.f, 0.f, 0.f, 1.f }; };

	miscCB.contents->dissolveStrength = 1.25f;
	isUseMyselfDissolveStrength = false;
	dissolveTex = "Noice.png";
	tiling = Vec2(1, 1);
	offset = Vec2(0, 0);
	miscCB.contents->tiling = Vec2(1, 1);

	shadowCaster = std::make_unique<ShadowCaster>();
	normalCaster = std::make_unique<NormalCaster>();
	silhouette = std::make_unique<Silhouette>();
	holographicCaster = std::make_unique<HolographicCaster>();
}

void Object3D::UpdateMatrix()
{
	Matrix mScale, mRot, mTrans;

	mScale = Matrix::Scale(scale);

	if (rotMode == RotMode::Quaternion)
	{
		mRot = rotation.GetRotMat();
	}
	else if (rotMode == RotMode::Euler)
	{
		mRot = Matrix::Identity();
		mRot *= Matrix::RotRollPitchYaw(rotationE);
	}

	mTrans = Matrix::Translation(position);

	matWorld = Matrix::Identity();
	matWorld *= mScale;
	matWorld *= mRot;
	matWorld *= mTrans;

	if (parent != nullptr)
	{
		matLocal = matWorld;
		matWorld *= parent->matWorld;
	}

	shadowCaster->worldMat = matWorld;
	normalCaster->worldMat = matWorld;
	holographicCaster->worldMat = matWorld;

	if (useSilhouette)
		silhouette->worldMat = matWorld;

	for (const auto& comp : components_)
	{
		Object3D* cast = dynamic_cast<Object3D*>(comp.get());
		if (cast)
		{
			cast->UpdateMatrix();
		}
	}
}

void Object3D::DecomposeMatrix()
{
	Vec3 axisX = matWorld.ExtractAxisX();
	Vec3 axisY = matWorld.ExtractAxisY();
	Vec3 axisZ = matWorld.ExtractAxisZ();

	scale.x = axisX.GetLength();
	scale.y = axisY.GetLength();
	scale.z = axisZ.GetLength();

	position.x = matWorld[3][0];
	position.y = matWorld[3][1];
	position.z = matWorld[3][2];

	axisX.Norm(scale.x);
	axisY.Norm(scale.y);
	axisZ.Norm(scale.z);

	Matrix wMatNoScale(
		static_cast<Float4>(static_cast<Float3>(axisX)),
		static_cast<Float4>(static_cast<Float3>(axisY)),
		static_cast<Float4>(static_cast<Float3>(axisZ)),
		{ position.x, position.y, position.z, 1.f }
	);

	rotationE.x = atan2f(wMatNoScale[1][2], wMatNoScale[2][2]);
	rotationE.y = atan2f(-wMatNoScale[0][2], sqrtf(wMatNoScale[1][2] * wMatNoScale[1][2] + wMatNoScale[2][2] * wMatNoScale[2][2]));
	rotationE.z = atan2f(wMatNoScale[0][1], wMatNoScale[0][0]);
}

void Object3D::Update()
{
	UpdateMatrix();
}

void Object3D::Draw()
{
	//モデルが設定されていないならなにもしない
	if (!model)
	{
		return;
	}

	//描画無効ならなにもしない
	if (disableDraw)
		return;

	if (!isUseMyselfDissolveStrength)
	{
		miscCB.contents->dissolveStrength = dissolveStrength;
	}

	miscCB.contents->tiling = tiling;

	shadowCaster->Draw(model);
	normalCaster->Draw(model);
	holographicCaster->Draw(model, texture);

	if (useSilhouette)
	{
		silhouette->Draw(model);
		return;
	}

	bool hasTexture = texture != "";

	//ブレンドモード・テクスチャ指定によって描画関数を変更
	switch (blendMode)
	{
	case Object3D::BlendMode::Opaque:
		if (hasTexture) {
			return Draw(texture);
		}
		break;
	case Object3D::BlendMode::Add:
		if (hasTexture) {
			return DrawAdd(texture);
		}
		else
		{
			return DrawAdd();
		}
		break;
	case Object3D::BlendMode::Alpha:
		if (hasTexture) {
			return DrawAlpha(texture);
		}
		else
		{
			return DrawAlpha();
		}
		break;
	case Object3D::BlendMode::Toon:
		if (hasTexture) {
			return DrawToon(texture);
		}
		else
		{
			return DrawToon();
		}
		break;
	case Object3D::BlendMode::PostRender:
		if (hasTexture) {
			return DrawPostRender(texture);
		}
		else
		{
			return DrawPostRender();
		}
	case Object3D::BlendMode::UIPlane:
		if (hasTexture) {
			return DrawUIPlane(texture);
		}
		else
		{
			return DrawToon();
		}
		break;
	case Object3D::BlendMode::TransparentWall:
		if (hasTexture) {
			return DrawTransparentWall(texture);
		}
		else
		{
			return DrawTransparentWall();
		}
		break;
	default:
		break;
	}

	//以下不透明テクスチャ指定なし描画
	transformCB.contents->mat = matWorld;

	SpRenderer::DrawCommand([&] {
		if (model->material.size())
		{
			GetSpDX()->cmdList->SetGraphicsRootDescriptorTable(1, SpTextureManager::GetGPUDescHandle(model->material.front().textureKey));
		}
		else
		{
			GetSpDX()->cmdList->SetGraphicsRootDescriptorTable(1, SpTextureManager::GetGPUDescHandle("notexture"));
		}

		GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(0, model->materialCBs.back().buffer->GetGPUVirtualAddress());

		GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(2, transformCB.buffer->GetGPUVirtualAddress());

		GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(4, brightnessCB.buffer->GetGPUVirtualAddress());

		GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(6, model->bMatrixCB.buffer->GetGPUVirtualAddress());

		GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(7, miscCB.buffer->GetGPUVirtualAddress());

		GetSpDX()->cmdList->SetGraphicsRootDescriptorTable(8, SpTextureManager::GetGPUDescHandle("dissolveMap"));

		GetSpDX()->cmdList->IASetVertexBuffers(0, 1, &model->vbView);

		GetSpDX()->cmdList->IASetIndexBuffer(&model->ibView);

		GetSpDX()->cmdList->DrawIndexedInstanced(model->ibView.SizeInBytes / sizeof(uint32_t), 1, 0, 0, 0);
		}, SpRenderer::Stage::Opaque, GetRT());
}

void Object3D::Draw(const TextureKey& key)
{
	transformCB.contents->mat = matWorld;
	miscCB.contents->tiling = tiling;
	miscCB.contents->offset = offset;

	//GetWDX()->cmdList->SetPipelineState(GPipeline::GetState("def"));
	//GetWDX()->cmdList->SetGraphicsRootSignature(SpRootSignature::Get("3D")->rootsignature.Get());

	//Light::Use();
	//Camera::UseCurrent();
	SpRenderer::DrawCommand([&, key] {
		GetSpDX()->cmdList->SetGraphicsRootDescriptorTable(1, SpTextureManager::GetGPUDescHandle(key));

		if (model->materialCBs.size())
			GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(0, model->materialCBs.front().buffer->GetGPUVirtualAddress());

		GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(2, transformCB.buffer->GetGPUVirtualAddress());

		GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(4, brightnessCB.buffer->GetGPUVirtualAddress());

		GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(6, model->bMatrixCB.buffer->GetGPUVirtualAddress());

		GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(7, miscCB.buffer->GetGPUVirtualAddress());

		GetSpDX()->cmdList->SetGraphicsRootDescriptorTable(8, SpTextureManager::GetGPUDescHandle(dissolveTex));

		GetSpDX()->cmdList->SetGraphicsRootDescriptorTable(9, SpTextureManager::GetGPUDescHandle("Black"));

		GetSpDX()->cmdList->IASetVertexBuffers(0, 1, &model->vbView);

		GetSpDX()->cmdList->IASetIndexBuffer(&model->ibView);

		GetSpDX()->cmdList->DrawIndexedInstanced(model->ibView.SizeInBytes / sizeof(uint32_t), 1, 0, 0, 0);
		}, SpRenderer::Stage::Opaque, GetRT());
}

void Object3D::DrawCommands(const TextureKey& key)
{
	GetSpDX()->cmdList->SetGraphicsRootDescriptorTable(1, SpTextureManager::GetGPUDescHandle(key));

	GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(0, model->materialCBs.front().buffer->GetGPUVirtualAddress());

	GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(2, transformCB.buffer->GetGPUVirtualAddress());

	GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(4, brightnessCB.buffer->GetGPUVirtualAddress());

	GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(5, model->bMatrixCB.buffer->GetGPUVirtualAddress());

	GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(6, miscCB.buffer->GetGPUVirtualAddress());

	GetSpDX()->cmdList->SetGraphicsRootDescriptorTable(7, SpTextureManager::GetGPUDescHandle("dissolveMap"));

	GetSpDX()->cmdList->IASetVertexBuffers(0, 1, &model->vbView);

	GetSpDX()->cmdList->IASetIndexBuffer(&model->ibView);

	GetSpDX()->cmdList->DrawIndexedInstanced(model->ibView.SizeInBytes / sizeof(uint32_t), 1, 0, 0, 0);
}

void Object3D::DrawAdd()
{
	if (model->material.size())
	{
		DrawAdd(model->material.front().textureKey);
	}
	else
	{
		DrawAdd("notexture");
	}
}

void Object3D::DrawAdd(const TextureKey& key)
{
	transformCB.contents->mat = matWorld;
	SpRenderer::DrawCommand([&] {
		GetSpDX()->cmdList->SetGraphicsRootDescriptorTable(1, SpTextureManager::GetGPUDescHandle(key));

		GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(0, model->materialCBs.front().buffer->GetGPUVirtualAddress());

		GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(2, transformCB.buffer->GetGPUVirtualAddress());

		GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(4, brightnessCB.buffer->GetGPUVirtualAddress());

		GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(5, model->bMatrixCB.buffer->GetGPUVirtualAddress());

		GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(6, miscCB.buffer->GetGPUVirtualAddress());

		GetSpDX()->cmdList->SetGraphicsRootDescriptorTable(7, SpTextureManager::GetGPUDescHandle("dissolveMap"));

		GetSpDX()->cmdList->IASetVertexBuffers(0, 1, &model->vbView);

		GetSpDX()->cmdList->IASetIndexBuffer(&model->ibView);

		GetSpDX()->cmdList->DrawIndexedInstanced(model->ibView.SizeInBytes / sizeof(uint32_t), 1, 0, 0, 0);
		}, SpRenderer::Stage::Add, GetRT());
}

void Object3D::DrawAlpha()
{
	if (model->material.size())
	{
		DrawAlpha(model->material.front().textureKey);
	}
	else
	{
		DrawAlpha("notexture");
	}
}

void Object3D::DrawAlpha(const TextureKey& key)
{
	transformCB.contents->mat = matWorld;
	miscCB.contents->tiling = tiling;
	miscCB.contents->offset = offset;
	this->alphaTexKey = key;
	SpRenderer::RegisterAlphaObj(this);
}

void Object3D::DrawToon()
{
	if (model->material.size())
	{
		DrawToon(model->material.front().textureKey);
	}
	else
	{
		DrawToon("notexture");
	}
}

void Object3D::DrawToon(const TextureKey& key)
{
	transformCB.contents->mat = matWorld;
	SpRenderer::DrawCommand([&] {

		if (GrayScale::GetIsGrayScale())
		{
			if (texType == 0)
			{
				GetSpDX()->cmdList->SetGraphicsRootDescriptorTable(1, SpTextureManager::GetGPUDescHandle(key));
			}
			// 真っ白
			else if (texType == 1)
			{
				GetSpDX()->cmdList->SetGraphicsRootDescriptorTable(1, SpTextureManager::GetGPUDescHandle("Block1.png"));
			}
			// 真っ黒
			else if (texType == 2)
			{
				GetSpDX()->cmdList->SetGraphicsRootDescriptorTable(1, SpTextureManager::GetGPUDescHandle("Block2.png"));
			}
		}
		else
		{
			GetSpDX()->cmdList->SetGraphicsRootDescriptorTable(1, SpTextureManager::GetGPUDescHandle(key));
		}

		if (model->materialCBs.size())
			GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(0, model->materialCBs.front().buffer->GetGPUVirtualAddress());

		GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(2, transformCB.buffer->GetGPUVirtualAddress());

		GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(4, brightnessCB.buffer->GetGPUVirtualAddress());

		GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(6, model->bMatrixCB.buffer->GetGPUVirtualAddress());

		GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(7, miscCB.buffer->GetGPUVirtualAddress());

		dissolveTex = "Noice.png";
		GetSpDX()->cmdList->SetGraphicsRootDescriptorTable(8, SpTextureManager::GetGPUDescHandle(dissolveTex));
		//GetSpDX()->cmdList->SetGraphicsRootDescriptorTable(8, SpTextureManager::GetGPUDescHandle("Noice.png"));

		GetSpDX()->cmdList->SetGraphicsRootDescriptorTable(9, SpTextureManager::GetGPUDescHandle("ShadowMap_depth_"));

		GetSpDX()->cmdList->IASetVertexBuffers(0, 1, &model->vbView);

		GetSpDX()->cmdList->IASetIndexBuffer(&model->ibView);

		GetSpDX()->cmdList->DrawIndexedInstanced(model->ibView.SizeInBytes / sizeof(uint32_t), 1, 0, 0, 0);
		}, SpRenderer::Stage::Toon, GetRT());
}

void Object3D::DrawUIPlane(const TextureKey& key)
{
	transformCB.contents->mat = matWorld;
	SpRenderer::DrawCommand([&] {
		GetSpDX()->cmdList->SetGraphicsRootDescriptorTable(1, SpTextureManager::GetGPUDescHandle(key));

		GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(0, model->materialCBs.front().buffer->GetGPUVirtualAddress());

		GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(2, transformCB.buffer->GetGPUVirtualAddress());

		GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(4, brightnessCB.buffer->GetGPUVirtualAddress());

		GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(5, model->bMatrixCB.buffer->GetGPUVirtualAddress());

		GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(6, miscCB.buffer->GetGPUVirtualAddress());

		GetSpDX()->cmdList->SetGraphicsRootDescriptorTable(7, SpTextureManager::GetGPUDescHandle("dissolveMap"));

		GetSpDX()->cmdList->IASetVertexBuffers(0, 1, &model->vbView);

		GetSpDX()->cmdList->IASetIndexBuffer(&model->ibView);

		GetSpDX()->cmdList->DrawIndexedInstanced(model->ibView.SizeInBytes / sizeof(uint32_t), 1, 0, 0, 0);
		}, SpRenderer::Stage::UIPlane, GetRT());
}

void Object3D::DrawPostRender()
{
	if (model->material.size())
	{
		DrawPostRender(model->material.front().textureKey);
	}
	else
	{
		DrawPostRender("notexture");
	}
}

void Object3D::DrawPostRender(const TextureKey& key)
{
	transformCB.contents->mat = matWorld;
	SpRenderer::DrawCommand([&] {
		GetSpDX()->cmdList->SetGraphicsRootDescriptorTable(1, SpTextureManager::GetGPUDescHandle(key));

		if (model->materialCBs.size())
			GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(0, model->materialCBs.front().buffer->GetGPUVirtualAddress());

		GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(2, transformCB.buffer->GetGPUVirtualAddress());

		GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(4, brightnessCB.buffer->GetGPUVirtualAddress());

		GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(6, model->bMatrixCB.buffer->GetGPUVirtualAddress());

		GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(7, miscCB.buffer->GetGPUVirtualAddress());

		GetSpDX()->cmdList->IASetVertexBuffers(0, 1, &model->vbView);

		GetSpDX()->cmdList->IASetIndexBuffer(&model->ibView);

		GetSpDX()->cmdList->DrawIndexedInstanced(model->ibView.SizeInBytes / sizeof(uint32_t), 1, 0, 0, 0);
		}, SpRenderer::Stage::PostRender, GetRT());
}

void Object3D::DrawTransparentWall()
{
	if (model->material.size())
	{
		DrawTransparentWall(model->material.front().textureKey);
	}
	else
	{
		DrawTransparentWall("notexture");
	}
}

void Object3D::DrawTransparentWall(const TextureKey& key)
{
	transformCB.contents->mat = matWorld;
	SpRenderer::DrawCommand([&] {
		GetSpDX()->cmdList->SetGraphicsRootDescriptorTable(1, SpTextureManager::GetGPUDescHandle(key));

		if (model->materialCBs.size())
			GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(0, model->materialCBs.front().buffer->GetGPUVirtualAddress());

		GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(2, transformCB.buffer->GetGPUVirtualAddress());

		GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(4, brightnessCB.buffer->GetGPUVirtualAddress());

		GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(6, model->bMatrixCB.buffer->GetGPUVirtualAddress());

		GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(7, miscCB.buffer->GetGPUVirtualAddress());

		GetSpDX()->cmdList->SetGraphicsRootDescriptorTable(8, SpTextureManager::GetGPUDescHandle("Black"));

		GetSpDX()->cmdList->SetGraphicsRootDescriptorTable(9, SpTextureManager::GetGPUDescHandle("ShadowMap_depth_"));

		GetSpDX()->cmdList->IASetVertexBuffers(0, 1, &model->vbView);

		GetSpDX()->cmdList->IASetIndexBuffer(&model->ibView);

		GetSpDX()->cmdList->DrawIndexedInstanced(model->ibView.SizeInBytes / sizeof(uint32_t), 1, 0, 0, 0);
		}, SpRenderer::Stage::TransparentWall, GetRT());
}

void Object3D::OnInspectorWindowDraw()
{

	if (ImGui::CollapsingHeader("Transform"))
	{
		ImGui::DragFloat3("Translation", &position.x);

		if (rotMode == RotMode::Euler)
		{
			ImGui::PushItemWidth(80.0f);
			SpImGui::DragAngle("##rotEX", &rotationE.x);
			ImGui::SameLine();
			SpImGui::DragAngle("##rotEY", &rotationE.y);
			ImGui::SameLine();
			SpImGui::DragAngle("Rotation", &rotationE.z);
			ImGui::PopItemWidth();
		}
		else
		{
			ImGui::Text("vvv Gizmo Does Not Work Currently vvv");
			ImGui::DragFloat4("Rotation", &rotation.w);
		}
		ImGui::Checkbox("Use Quaternion Rotation", reinterpret_cast<bool*>(&rotMode));

		ImGui::DragFloat3("Scale", &scale.x);
	}
	ImGui::Separator();

	if (ImGui::CollapsingHeader("Render"))
	{
		int blendModeInt = (int)blendMode;
		ImGui::RadioButton("Opaque", &blendModeInt, (int)BlendMode::Opaque);	ImGui::SameLine();
		ImGui::RadioButton("Add", &blendModeInt, (int)BlendMode::Add);			ImGui::SameLine();
		ImGui::RadioButton("Alpha", &blendModeInt, (int)BlendMode::Alpha);		ImGui::SameLine();
		ImGui::RadioButton("Toon", &blendModeInt, (int)BlendMode::Toon);
		ImGui::RadioButton("PostRender", &blendModeInt, (int)BlendMode::PostRender); ImGui::SameLine();
		ImGui::RadioButton("UIPlane", &blendModeInt, (int)BlendMode::UIPlane);
		ImGui::RadioButton("TransparentWall", &blendModeInt, (int)BlendMode::TransparentWall);
		blendMode = (BlendMode)blendModeInt;
		ImGui::Checkbox("Disable Draw", &disableDraw);
		ImGui::Separator();

		ImGui::ColorEdit4("Brightness", reinterpret_cast<float*>(brightnessCB.contents));

		ImGui::Checkbox("Use Silhouette", &useSilhouette);

		SpImGui::InputText("RenderTarget", renderTarget);
	}
	ImGui::Separator();

	if (ImGui::CollapsingHeader("Texture"))
	{
		ImGui::RadioButton("None", &texType, 0);	ImGui::SameLine();
		ImGui::RadioButton("Black", &texType, 1);	ImGui::SameLine();
		ImGui::RadioButton("White", &texType, 2);	ImGui::Separator();

		const size_t bufSize = 256;
		static char buf[bufSize];
		strncpy_s(buf, texture.c_str(), std::min(bufSize, texture.length()));
		if (ImGui::InputText("Texture Key", buf, bufSize, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			texture = buf;
		};
		ImGui::DragFloat2("Tiling", &tiling.x);
		ImGui::DragFloat2("Offset", &offset.x);

		static std::string input = dissolveTex;
		if (SpImGui::InputText("Dissolve Tex Key", input))
		{
			dissolveTex = input;
		}

		ImGui::Checkbox("UseMyself", &isUseMyselfDissolveStrength);
		ImGui::DragFloat("Disssolve", &miscCB.contents->dissolveStrength, 0.01f, 0.f, 1.25f);
	}
	ImGui::Separator();

	if (ImGui::CollapsingHeader("Model"))
	{
		if (model)
		{
			ImGui::Text("ModelName");
			ImGui::Text("%s", model->key.c_str());
			ImGui::SameLine();

			if (ImGui::Button("Reset")) {
				model = nullptr;
			}
		}

		if (model)
		{
			if (ImGui::RadioButton("Default", &normalType, 0))
			{
				model->MappingVertex();
			}
			ImGui::SameLine();
			if (ImGui::RadioButton("Smoothing", &normalType, 1))
			{
				model->MappingSmoothing();
			}
		}
	}
	ImGui::Separator();

	UpdateMatrix();
}

void Object3D::DrawGizmo()
{
	ImGuizmo::SetDrawlist();
	ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y,
		ImGui::GetWindowWidth(), ImGui::GetWindowHeight());

	ImGuizmo::AllowAxisFlip(true);

	static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);

	static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);

	ImGuizmo::Enable(true);

	if (!Input::Key::Down(DIK_LCONTROL) && !Input::Key::Down(DIK_RCONTROL))
	{
		if (Input::Key::Triggered(DIK_T))
		{
			mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
			mCurrentGizmoMode = ImGuizmo::WORLD;
		}
		else if (Input::Key::Triggered(DIK_R))
		{
			mCurrentGizmoOperation = ImGuizmo::ROTATE;
			mCurrentGizmoMode = ImGuizmo::WORLD;
		}
		else if (Input::Key::Triggered(DIK_S))
		{
			mCurrentGizmoOperation = ImGuizmo::SCALE;
			mCurrentGizmoMode = ImGuizmo::WORLD;
		}

		if (Input::Key::DoubleTriggered(DIK_T))
		{
			mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
			mCurrentGizmoMode = ImGuizmo::LOCAL;
		}
		else if (Input::Key::DoubleTriggered(DIK_R))
		{
			mCurrentGizmoOperation = ImGuizmo::ROTATE;
			mCurrentGizmoMode = ImGuizmo::LOCAL;
		}
		else if (Input::Key::DoubleTriggered(DIK_S))
		{
			mCurrentGizmoOperation = ImGuizmo::SCALE;
			mCurrentGizmoMode = ImGuizmo::LOCAL;
		}
	}

	Matrix view = Camera::sCurrent->GetViewMat();
	Matrix proj = Camera::sCurrent->GetProjMat();

	UpdateMatrix();

	float snap[3] = { 0.001f,0.001f,0.001f };
	if (Input::Key::Down(DIK_LSHIFT))
	{
		snap[0] = 1.0f;
		snap[1] = 1.0f;
		snap[2] = 1.0f;
	}

	Matrix mat = matWorld;
	if (parent != nullptr)
	{
		mat = matWorld;
	}


	ImGuizmo::Manipulate(reinterpret_cast<float*>(&view),
		reinterpret_cast<float*>(&proj), mCurrentGizmoOperation, mCurrentGizmoMode, &mat[0][0], NULL, snap);

	if (ImGuizmo::IsUsing())
	{
		//mat.DecomposeTransform(&position, &rot, &scale);

		//Vec3 deltaRot = static_cast<Vec3>(rot) - rotationE;

		//rotationE += deltaRot;

		Float3 rot;

		if (parent)
		{
			mat *= -parent->matWorld;
		}

		mat.DecomposeTransform(&position, &rot, &scale);
		UpdateMatrix();
	}
}

void Object3D::ReadParamJson(const nlohmann::json& jsonObject)
{
	if (jsonObject.contains("Name"))
	{
		name_ = jsonObject["Name"];
	}

	if (jsonObject.contains("DisableDraw"))
	{
		disableDraw = jsonObject["DisableDraw"];
	}
	if (jsonObject.contains("RenderTarget"))
	{
		renderTarget = jsonObject["RenderTarget"];
	}

	if (jsonObject.contains("Position"))
	{
		position.x = jsonObject["Position"]["X"];
		position.y = jsonObject["Position"]["Y"];
		position.z = jsonObject["Position"]["Z"];
	}

	if (jsonObject.contains("Scale"))
	{
		scale.x = jsonObject["Scale"]["X"];
		scale.y = jsonObject["Scale"]["Y"];
		scale.z = jsonObject["Scale"]["Z"];
	}

	if (jsonObject.contains("Rotation"))
	{
		rotation.v.x = jsonObject["Rotation"]["X"];
		rotation.v.y = jsonObject["Rotation"]["Y"];
		rotation.v.z = jsonObject["Rotation"]["Z"];
		rotation.w = jsonObject["Rotation"]["W"];
	}

	if (jsonObject.contains("RotationEuler"))
	{
		rotationE.x = jsonObject["RotationEuler"]["X"];
		rotationE.y = jsonObject["RotationEuler"]["Y"];
		rotationE.z = jsonObject["RotationEuler"]["Z"];
	}

	if (jsonObject.contains("Brightness"))
	{
		brightnessCB.contents->x = jsonObject["Brightness"]["X"];
		brightnessCB.contents->y = jsonObject["Brightness"]["Y"];
		brightnessCB.contents->z = jsonObject["Brightness"]["Z"];
		brightnessCB.contents->w = jsonObject["Brightness"]["W"];
	}

	texture = jsonObject["Texture"];
	if (jsonObject.contains("TexType"))
	{
		texType = jsonObject["TexType"];
	}
	if (jsonObject.contains("DissolveTex"))
	{
		if (jsonObject.contains("Tiling"))
		{
			tiling.x = jsonObject["Tiling"]["X"];
			tiling.y = jsonObject["Tiling"]["Y"];
		}

		dissolveTex = jsonObject["DissolveTex"];
		if (jsonObject.contains("DissolveStrength"))
		{
			miscCB.contents->dissolveStrength = jsonObject["DissolveStrength"];
		}
		if (jsonObject.contains("UseMyselfDissolveStrength"))
		{
			isUseMyselfDissolveStrength = jsonObject["UseMyselfDissolveStrength"];
		}
	}

	model = nullptr;
	if (jsonObject.contains("Model"))
	{
		std::string modelStr = jsonObject.At("Model");
		model = ModelManager::GetModel(modelStr);
	}

	if (jsonObject.contains("BlendMode"))
	{
		blendMode = (BlendMode)jsonObject["BlendMode"];
	}
	if (model)
	{
		if (jsonObject.contains("NormalType"))
		{
			normalType = jsonObject["NormalType"];
			if (normalType == 0)
			{
				model->MappingVertex();
			}
			else
			{
				model->MappingSmoothing();
			}
		}
	}

	if (jsonObject.contains("RenderTarget"))
	{
		renderTarget = jsonObject["RenderTarget"];
	}
}

void Object3D::WriteParamJson(nlohmann::json& jsonObject)
{
	jsonObject["Name"] = name_;
	jsonObject["DisableDraw"] = disableDraw;

	jsonObject["Position"]["X"] = position.x;
	jsonObject["Position"]["Y"] = position.y;
	jsonObject["Position"]["Z"] = position.z;

	jsonObject["Scale"]["X"] = scale.x;
	jsonObject["Scale"]["Y"] = scale.y;
	jsonObject["Scale"]["Z"] = scale.z;

	jsonObject["Rotation"]["X"] = rotation.v.x;
	jsonObject["Rotation"]["Y"] = rotation.v.y;
	jsonObject["Rotation"]["Z"] = rotation.v.z;
	jsonObject["Rotation"]["W"] = rotation.w;

	jsonObject["RotationEuler"]["X"] = rotationE.x;
	jsonObject["RotationEuler"]["Y"] = rotationE.y;
	jsonObject["RotationEuler"]["Z"] = rotationE.z;

	jsonObject["Brightness"]["X"] = brightnessCB.contents->x;
	jsonObject["Brightness"]["Y"] = brightnessCB.contents->y;
	jsonObject["Brightness"]["Z"] = brightnessCB.contents->z;
	jsonObject["Brightness"]["W"] = brightnessCB.contents->w;

	jsonObject["Texture"] = texture;
	jsonObject["TexType"] = texType;
	jsonObject["Tiling"]["X"] = tiling.x;
	jsonObject["Tiling"]["Y"] = tiling.y;
	jsonObject["DissolveTex"] = dissolveTex;
	jsonObject["DissolveStrength"] = miscCB.contents->dissolveStrength;
	jsonObject["UseMyselfDissolveStrength"] = isUseMyselfDissolveStrength;


	if (model)
	{
		jsonObject["Model"] = model->key;
		jsonObject["NormalType"] = normalType;
	}
	else
	{
		jsonObject["Model"] = "";
	}

	jsonObject["BlendMode"] = (int)blendMode;
	jsonObject["RenderTarget"] = renderTarget;
}

void Object3D::CopyComponent(IComponent* src)
{
	Object3D* cast = dynamic_cast<Object3D*>(src);
	name_ = cast->name_;
	scale = cast->scale;
	rotation = cast->rotation;
	rotationE = cast->rotationE;
	rotMode = cast->rotMode;
	blendMode = cast->blendMode;
	position = cast->position;
	parent = cast->parent;
	texture = cast->texture;
	model = cast->model;
	distanceToCam = cast->distanceToCam;
	alphaTexKey = cast->alphaTexKey;
	tags = cast->tags;
	*brightnessCB.contents = *cast->brightnessCB.contents;

	UpdateMatrix();
}

TextureKey Object3D::GetRT()
{
	if (SpTextureManager::IsMasterTexture(renderTarget))
	{
		return renderTarget;
	}

	return RTVManager::defaultRT;
}
