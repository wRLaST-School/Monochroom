#include "stdafx.h"
#include "SrSpriteStage.h"
#include <Sprite.h>
#include <SpDS.h>
#include <DockPanel.h>
#include <NoEffect.h>
#include <SceneManager.h>
#include <RTVManager.h>

void SrSpriteStage::DrawCommands(std::function<void(void)> cmd, TextureKey rt)
{
	auto itr = commands_.find(rt);
	if (itr != commands_.end())
		itr->second.push_back(cmd);
}

void SrSpriteStage::Init()
{
}

void SrSpriteStage::PreDraw()
{
	Sprite::PreSpriteDraw();	
	if (SceneManager::currentScene->GetName() == "Title" ||
		SceneManager::currentScene->GetName() == "StageSelect")
	{
		RTVManager::SetRenderTargetToTexture("RGBShiftF", false);

	}
	else
	{
		RTVManager::SetRenderTargetToTexture("KawaseBloomP3", false);
	}
}

void SrSpriteStage::PostDraw()
{
	Sprite::PostSpriteDraw();
}

void SrSpriteStage::Render()
{
	for (auto& rt : commands_)
	{
		//RTVManager::SetRenderTargetToTexture(rt.first, false);

		for (auto& cmd : rt.second)
		{
			cmd();
		}
	}
	commands_.clear();

	SpDS::Render();
}
