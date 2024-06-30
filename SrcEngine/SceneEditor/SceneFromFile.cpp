#include "stdafx.h"
#include "SceneFromFile.h"
#include <SceneRW.h>
#include <DockPanel.h>
#include <SceneManager.h>
#include <ScriptComponent.h>

SceneFromFile::SceneFromFile(std::string filePath)
{
	filePath_ = filePath;

	DockPanel::lastSavePath = filePath;

	if (filePath == "Assets/Scene/Empty.scene") DockPanel::lastSavePath = "";
}

void SceneFromFile::LoadResources()
{
	SceneRW::LoadScene(this, filePath_);
}

void SceneFromFile::Init()
{
	for (auto& c : components_)
	{
		AwakeAllChildComponents(c.second.get());
	}

	for (auto& c : components_)
	{
		InitAllChildComponents(c.second.get());
	}

	auto scripts = SceneManager::FindScripts();

	for (auto& s : scripts) {
		s->CastTo<ScriptComponent>()->AwakeScript();
	}
	for (auto& s : scripts) {
		s->CastTo<ScriptComponent>()->InitScript();
	}
}

void SceneFromFile::Update()
{
}

void SceneFromFile::DrawBack()
{
}

void SceneFromFile::Draw3D()
{
}

void SceneFromFile::DrawSprite()
{
}
