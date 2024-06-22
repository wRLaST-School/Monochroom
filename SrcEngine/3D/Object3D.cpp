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

Object3D::Object3D()
{
	{ transformCB.contents->mat = Matrix::Identity(); *brightnessCB.contents = { 1.0f, 1.0f, 1.0f, 1.0f }; miscCB.contents->rimColor = { 1.f, 0.f, 0.f, 1.f }; };
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
		matWorld *= parent->matWorld;
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
	float radius = Vec3(scale).GetMaxElement();
	bool isInside = Camera::sCurrent->CheckisInCameraInside(position, radius);
	if (!isInside)
	{
		return;
	}

	//モデルが設定されていないならなにもしない
	if (!model)
	{
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
		}, SpRenderer::Stage::Opaque);
}

void Object3D::Draw(const TextureKey& key)
{
	transformCB.contents->mat = matWorld;

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

		GetSpDX()->cmdList->SetGraphicsRootDescriptorTable(8, SpTextureManager::GetGPUDescHandle("dissolveMap"));

		GetSpDX()->cmdList->IASetVertexBuffers(0, 1, &model->vbView);

		GetSpDX()->cmdList->IASetIndexBuffer(&model->ibView);

		GetSpDX()->cmdList->DrawIndexedInstanced(model->ibView.SizeInBytes / sizeof(uint32_t), 1, 0, 0, 0);
		}, SpRenderer::Stage::Opaque);
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
		}, SpRenderer::Stage::Add);
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
	this->alphaTexKey = key;
	SpRenderer::RegisterAlphaObj(this);
}

void Object3D::DrawToon()
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

void Object3D::DrawToon(const TextureKey& key)
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
		}, SpRenderer::Stage::Toon);
}

void Object3D::OnInspectorWindowDraw()
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
		ImGui::DragFloat4("Rotation", &rotation.w);
		ImGui::Text("vvv Gizmo Does Not Work Currently vvv");
	}
	ImGui::Checkbox("Use Quaternion Rotation", reinterpret_cast<bool*>(&rotMode));

	ImGui::DragFloat3("Scale", &scale.x);

	ImGui::Separator();

	ImGui::ColorEdit4("Brightness", reinterpret_cast<float*>(brightnessCB.contents));

	ImGui::Separator();

	const size_t bufSize = 256;
	char buf[bufSize];
	strncpy_s(buf, texture.c_str(), std::min(bufSize, texture.length()));
	if (ImGui::InputText("Texture", buf, bufSize, ImGuiInputTextFlags_EnterReturnsTrue))
	{
		texture = buf;
	};

	if (ImGui::CollapsingHeader("Blend Mode"))
	{
		int blendeModeInt = (int)blendMode;
		ImGui::RadioButton("Opaque", &blendeModeInt, (int)BlendMode::Opaque);	ImGui::SameLine();
		ImGui::RadioButton("Add", &blendeModeInt, (int)BlendMode::Add);			ImGui::SameLine();
		ImGui::RadioButton("Alpha", &blendeModeInt, (int)BlendMode::Alpha);		ImGui::SameLine();
		ImGui::RadioButton("Toon", &blendeModeInt, (int)BlendMode::Toon);
		blendMode = (BlendMode)blendeModeInt;
	}

	ImGui::Separator();

	if (ImGui::CollapsingHeader("Model"))
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
		snap[0] = 0.1f;
		snap[1] = 0.1f;
		snap[2] = 0.1f;
	}

	ImGuizmo::Manipulate(reinterpret_cast<float*>(&view),
		reinterpret_cast<float*>(&proj), mCurrentGizmoOperation, mCurrentGizmoMode, &matWorld[0][0], NULL, snap);

	if (ImGuizmo::IsUsing())
	{
		Float3 rot;
		matWorld.DecomposeTransform(&position, &rot, &scale);

		//Float3 rot;
		//ImGuizmo::DecomposeMatrixToComponents(&matWorld[0][0] ,&position.x, &rot.x, &scale.x);
		//ImGuizmo::RecomposeMatrixFromComponents(&position.x, &rot.x, &scale.x, &matWorld[0][0]);
		//rotationE = ConvertRadianToAngle(rot);

		Vec3 deltaRot = static_cast<Vec3>(rot) - rotationE;

		rotationE += deltaRot;

		UpdateMatrix();
	}
}

void Object3D::ReadParamJson(const nlohmann::json& jsonObject)
{
	if (jsonObject.contains("Name"))
	{
		name_ = jsonObject["Name"];
	}

	position.x = jsonObject["Position"]["X"];
	position.y = jsonObject["Position"]["Y"];
	position.z = jsonObject["Position"]["Z"];

	scale.x = jsonObject["Scale"]["X"];
	scale.y = jsonObject["Scale"]["Y"];
	scale.z = jsonObject["Scale"]["Z"];

	rotation.v.x = jsonObject["Rotation"]["X"];
	rotation.v.y = jsonObject["Rotation"]["Y"];
	rotation.v.z = jsonObject["Rotation"]["Z"];
	rotation.w = jsonObject["Rotation"]["W"];

	rotationE.x = jsonObject["RotationEuler"]["X"];
	rotationE.y = jsonObject["RotationEuler"]["Y"];
	rotationE.z = jsonObject["RotationEuler"]["Z"];

	texture = jsonObject["Texture"];
	std::string modelStr = jsonObject.At("Model");

	if (jsonObject.contains("BlendMode"))
	{
		blendMode = (BlendMode)jsonObject["BlendMode"];
	}
	model = ModelManager::GetModel(modelStr);
	if (model)
	{
		if (jsonObject.contains("NormalType"))
		{
			normalType = jsonObject["NormalType"];
		}
	}
}

void Object3D::WriteParamJson(nlohmann::json& jsonObject)
{
	jsonObject["Name"] = name_;

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

	jsonObject["Texture"] = texture;

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

	UpdateMatrix();
}
