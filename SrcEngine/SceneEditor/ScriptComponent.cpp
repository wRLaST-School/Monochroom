#include "stdafx.h"
#include "ScriptComponent.h"
#include <SpImGui.h>
#include <Libra/LibraCompiler/LibraCompiler.h>
#include <Libra/LibraCompiler/CPPCompiler.h>
#include <format>
#include <functional>
#include <SceneManager.h>

void ScriptComponent::Init()
{
	LoadDLL(true);
}

void ScriptComponent::Update()
{
	if (dllobj_.GetComponent()) dllobj_.GetComponent()->Update();
}

void ScriptComponent::Draw()
{
	if (dllobj_.GetComponent()) dllobj_.GetComponent()->Draw();
}

void ScriptComponent::OnInspectorWindowDraw()
{
	if (dllobj_.GetComponent()) dllobj_.GetComponent()->OnInspectorWindowDraw();

	if (SpImGui::InputText("Class Name", &className, ImGuiInputTextFlags_::ImGuiInputTextFlags_EnterReturnsTrue))
	{
		Init();
		dllobj_.GetComponent()->Awake();
		dllobj_.GetComponent()->Init();
	};

	if (ImGui::Button("Compile"))
	{
		CompileScript();
	}
}

void ScriptComponent::WriteParamJson(nlohmann::json& jsonObj)
{
	jsonObj["Name"] = name_;
	jsonObj["filePath"] = className;
}

void ScriptComponent::ReadParamJson(const nlohmann::json& jsonObj)
{
	if (jsonObj.contains("Name"))
	{
		name_ = jsonObj["Name"];
	}
	className = jsonObj["filePath"];
}

void ScriptComponent::CopyComponent(IComponent* src)
{
	ScriptComponent* cast = dynamic_cast<ScriptComponent*>(src);
	name_ = cast->name_;
	className = cast->className;
}

void ScriptComponent::AwakeScript()
{
	if (dllobj_.GetComponent()) dllobj_.GetComponent()->Awake();
}

void ScriptComponent::InitScript()
{
	if (dllobj_.GetComponent()) dllobj_.GetComponent()->Init();
}

Libra::DLLObject* ScriptComponent::GetObj()
{
	return &dllobj_;
}

ScriptComponent::~ScriptComponent()
{
}

void ScriptComponent::CompileScript()
{
	//スクリプトコンポーネントを全て保持してdllobjをfree
	eastl::vector<ScriptComponent*> scriptcomps;

	std::function<void(eastl::vector<ScriptComponent*>&, IComponent*)> GetAllScriptCompsRecursive =
		[&](eastl::vector<ScriptComponent*>& list, IComponent* parent) {
		auto& components = parent->GetAllComponents();
		if (components.size())
		{
			for (auto itr = components.begin(); itr != components.end(); itr++)
			{
				GetAllScriptCompsRecursive(list, itr->get());
			}
		}

		auto castedParent = parent->CastTo<ScriptComponent>();
		if (castedParent) {
			list.push_back(castedParent);
		}
	};

	GetAllScriptCompsRecursive(scriptcomps, SceneManager::currentScene.get());

	//スクリプトコンポーネントを全て保持して中身をdeleteしてからdllobjをfree
	for (auto& c : scriptcomps)
	{
		c->dllobj_.DeleteComponent();
	}

	for (auto& c : scriptcomps) {
		if (c->dllobj_.GetModule())
			c->dllobj_.Free();
	}

	//コンパイル処理
	//Libra::Compiler::Compile(filePath, compileDest);

	CPPCompiler::Compile();
	for (auto& c : scriptcomps) {
		c->LoadDLL(true);
	}
	for (auto& c : scriptcomps) {
		c->AwakeScript();
	}
	for (auto& c : scriptcomps) {
		c->InitScript();
	}
}

void ScriptComponent::LoadDLL(bool noInit)
{
	//読み込み処理
	dllobj_.LoadDLL(className);

	if (dllobj_.GetComponent()) dllobj_.GetComponent()->body = this;
	else OutputDebugStringA(std::format("No Such Class Found. Class Name: {}\n", className).c_str());

	if (!noInit && dllobj_.GetComponent()) {
		dllobj_.GetComponent()->Awake();
		dllobj_.GetComponent()->Init();
	}
}
