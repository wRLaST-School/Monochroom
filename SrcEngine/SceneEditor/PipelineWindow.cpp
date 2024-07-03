#include "stdafx.h"
#include "PipelineWindow.h"
#include <SpImGui.h>
#include <GPipeline.h>
#include <Model.h>
#include <Sprite.h>

std::optional<std::string> PipelineWindow::sPipelineID;

PipelineWindow::PipelineWindow()
{
	mIsOpen = true;
}

void PipelineWindow::DrawWindow()
{
	SpImGui::Command([&]
		{
			if (!mIsOpen)
			{
				return;
			}

			ImGui::Begin("Pipeline Window", &mIsOpen);

			//タブ
			if (ImGui::BeginTabBar("##PipeWindowTabs", ImGuiTabBarFlags_None))
			{
				if (ImGui::BeginTabItem("Pipeline"))
				{
					tabState = TabState::Pipeline;
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("RootSignature"))
				{
					tabState = TabState::RootSignature;
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Shader"))
				{
					tabState = TabState::Shader;
					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}

			switch (tabState)
			{
			case TabState::Pipeline:
				PipelineTabStateDraw();
				break;

			case TabState::RootSignature:
				RootSignatureTabStateDraw();
				break;

			case TabState::Shader:
				ShaderTabStateDraw();
				break;
			}

			ImGui::End();
		});
}

void PipelineWindow::PipelineTabStateDraw()
{
	PipelineDesc desc;

	if (SpImGui::TreeNode("InputLayout", true))
	{
		static int cur = 0;
		ImGui::RadioButton("ModelCommon", &cur, 0); ImGui::SameLine();
		ImGui::RadioButton("SpriteCommon", &cur, 1);

		if (cur == 0)
		{
			desc.Render.InputLayout.pInputElementDescs = ModelCommon::inputLayout;
			desc.Render.InputLayout.NumElements = _countof(ModelCommon::inputLayout);
		}
		else if (cur == 1)
		{
			desc.Render.InputLayout.pInputElementDescs = SpriteCommon::inputLayout2D;
			desc.Render.InputLayout.NumElements = _countof(SpriteCommon::inputLayout2D);
		}

		ImGui::TreePop();
	}
	ImGui::Separator();

	if (SpImGui::TreeNode("CullMode", true))
	{
		static int cur = 0;
		ImGui::RadioButton("Cull Back", &cur, 0);	 ImGui::SameLine();
		ImGui::RadioButton("Cull Front", &cur, 1);	 ImGui::SameLine();
		ImGui::RadioButton("Cull None", &cur, 2);

		if (cur == 0)
		{
			desc.Render.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;

		}
		else if (cur == 1)
		{
			desc.Render.RasterizerState.CullMode = D3D12_CULL_MODE_FRONT;
		}
		else if (cur == 2)
		{
			desc.Render.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
		}

		ImGui::TreePop();
	}
	ImGui::Separator();

	if (SpImGui::TreeNode("Shader", true))
	{
		static std::string shaderID;
		SpImGui::InputText("Shader ID", shaderID);

		SpShader* shader = GetShader(shaderID);
		if (shader)
		{
			desc.Shader.pShader = shader;
		}
		else
		{
			if (!shaderID.empty())
			{
				ImGui::Text("Shader Not Found.");
			}
		}
		ImGui::TreePop();
	}
	ImGui::Separator();

	if (SpImGui::TreeNode("RootSignature", true))
	{
		static std::string rootSignatureID;
		SpImGui::InputText("RootSignature ID", rootSignatureID);

		SpRootSignature* rootSignature = SpRootSignature::Get(rootSignatureID);
		if (rootSignature)
		{
			desc.RootSignature.ptr = rootSignature->rootsignature.Get();
		}
		else
		{
			if (!rootSignatureID.empty())
			{
				ImGui::Text("RootSignature Not Found.");
			}
		}
		ImGui::TreePop();
	}
	ImGui::Separator();

	static std::string pipelineID;
	SpImGui::InputText("Pipeline ID", pipelineID);
	if (ImGui::Button("Create", ImVec2(128, 32)))
	{
		GPipeline::Create(desc, pipelineID);
	}
}

void PipelineWindow::RootSignatureTabStateDraw()
{
	//if (ImGui::SmallButton("+"))
	//{
	//	creatingRS.params.emplace_back();
	//}
	//ImGui::SameLine();
	//if (ImGui::SmallButton("-"))
	//{
	//	creatingRS.params.pop_back();
	//}

	//for (int i = 0; i < creatingRS.params.size(); i++)
	//{
	//	std::string label = "Param[" + std::to_string(i) + "]";
	//	if (SpImGui::TreeNode(label.c_str(), false))
	//	{

	//	}
	//}

	//static std::string rsID;
	//SpImGui::InputText("RootSignature ID", rsID);
	//if (ImGui::Button("Create", ImVec2(128, 32)))
	//{
	//	// サンプラー設定
	//	creatingRS.UseDefaultSettings();
	//}
}

void PipelineWindow::ShaderTabStateDraw()
{
	//static std::string vsPath;
	//SpImGui::InputText("VS Path", vsPath);
	//static std::string psPath;
	//SpImGui::InputText("PS Path", psPath);
	//static std::string gsPath;
	//SpImGui::InputText("GS Path", gsPath);

	//static std::string shaderRegisteID;
	//SpImGui::InputText("Shader Registe ID", shaderRegisteID);
	//if (ImGui::Button("Register", ImVec2(128, 32)))
	//{
	//	RegisterShader(shaderRegisteID);
	//	InitVS(shaderRegisteID, vsPath);
	//	InitPS(shaderRegisteID, psPath);
	//	InitGS(shaderRegisteID, gsPath);
	//}

	//ImGui::Separator();

	static std::string shaderCompileID;
	static std::string message;
	static bool flag = false;
	SpImGui::InputText("Shader Compile ID", shaderCompileID);
	if (ImGui::Button("Compile", ImVec2(128, 32)))
	{
		message.clear();
		flag = ReCompile(shaderCompileID);

		// コンパイル成功したなら
		if (flag)
		{
			for (const auto& pso : *GetPSOMap())
			{
				if (pso.second.shaderID == shaderCompileID)
				{
					sPipelineID = pso.second.psoID;
				}
			}
		}
	}

	if (flag)
	{
		message = "Compilation was successful.";
		ImGui::Text(message.c_str());
	}
	else
	{
		message = "Compilation failed";
		ImGui::Text(message.c_str());
	}
}

void PipelineWindow::SDraw()
{
	GetInstance()->DrawWindow();
}

void PipelineWindow::ReCompileGPipeline()
{
	if (!sPipelineID.has_value())
	{
		return;
	}

	GPipeline::ReCreate(sPipelineID.value());

	sPipelineID.reset();
}

void PipelineWindow::SetisOpen(bool isOpen)
{
	mIsOpen = isOpen;
}

bool PipelineWindow::GetisOpen()
{
	return mIsOpen;
}

PipelineWindow* PipelineWindow::GetInstance()
{
	static PipelineWindow ins;
	return &ins;
}


