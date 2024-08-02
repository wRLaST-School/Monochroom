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

	SoundManager::LoadWave("Assets/Sounds/blink.wav", "Blink");
	SoundManager::LoadWave("Assets/Sounds/decide.wav", "Decide");
	SoundManager::LoadWave("Assets/Sounds/generate.wav", "Generate");
	SoundManager::LoadWave("Assets/Sounds/select.wav", "Select");
	SoundManager::LoadWave("Assets/Sounds/goal.wav", "Goal");
}

void SceneFromFile::Init()
{
	for (auto& c : components_)
	{
		AwakeAllChildComponents(c.get());
	}

	for (auto& c : components_)
	{
		InitAllChildComponents(c.get());
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
