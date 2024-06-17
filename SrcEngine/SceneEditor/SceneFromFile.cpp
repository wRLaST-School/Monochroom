#include "stdafx.h"
#include "SceneFromFile.h"
#include <SceneRW.h>
#include <DockPanel.h>

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
		InitAllChildComponents(c.second.get());
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
