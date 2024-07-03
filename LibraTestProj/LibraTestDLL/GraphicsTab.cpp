#include "GraphicsTab.h"
#include <Camera.h>
#include <SpDS.h>
#include <SpTextureManager.h>
#include <Util.h>
#include <SceneManager.h>
#include <Input.h>
#include <ConsoleWindow.h>
#include "GameManager.h"


void GraphicsTab::Init()
{
	mGraphicsOptionNum = 2;

	mGraphicUIObj.resize(mGraphicsOptionNum);

	// グラフィックス項目のオブジェクト設定
	mGraphicUIObj[POSTPROCESS].planeObj = SceneManager::FindObject<Object3D>("PostProcess");
	mGraphicUIObj[POSTPROCESS].planeObj = SceneManager::FindObject<Object3D>("Shadow");
}

void GraphicsTab::Update()
{
}

void GraphicsTab::Draw()
{
}

void GraphicsTab::OnUpdate()
{
	for (size_t i = 0; i < mGraphicsOptionNum; i++)
	{
		mGraphicUIObj[i].planeObj->Activate();
	}

	mCurrentNum = 0;
}

void GraphicsTab::OffUpdate()
{
	for (size_t i = 0; i < mGraphicsOptionNum; i++)
	{
		mGraphicUIObj[i].planeObj->Deactivate();
	}

	mCurrentNum = 0;
}
