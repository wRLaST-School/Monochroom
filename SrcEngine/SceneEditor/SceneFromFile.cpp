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

	SoundManager::LoadWave("Assets/Sounds/blink.wav", "Blink"); // トランジション
	SoundManager::LoadWave("Assets/Sounds/decide.wav", "Decide"); // 決定音
	SoundManager::LoadWave("Assets/Sounds/generate.wav", "Generate"); // ステージ生成音
	SoundManager::LoadWave("Assets/Sounds/select.wav", "Select"); // 選択(上下左右)音
	SoundManager::LoadWave("Assets/Sounds/goal.wav", "Goal"); // ゴールしたとき
	SoundManager::LoadWave("Assets/Sounds/goggle.wav", "Goggle"); // ゴーグルつけたとき
	SoundManager::LoadWave("Assets/Sounds/kinesis.wav", "Kinesis"); // 引き寄せるとき
	SoundManager::LoadWave("Assets/Sounds/presplate.wav", "PresPlate"); // ステージのボタンに乗った時
	SoundManager::LoadWave("Assets/Sounds/jump.wav", "Jump"); // ジャンプ時
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
