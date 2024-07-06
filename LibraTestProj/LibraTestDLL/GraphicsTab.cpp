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

	mGraphicsFlags.resize(mGraphicsOptionNum);

	mGraphicsNoneSelectScale = { 0.9f,0.6f,1 };
	mGraphicsSelectScale = { 1,0.8f,1 };

	mGraphicsDefuValue = 1;
	mGraphicsMaxValue = 10;

	mGraphicsUIObj.resize(mGraphicsOptionNum);

	mItemsParentObj = std::make_unique<Object3D>();
	mItemsParentObj.reset(SceneManager::FindObject<Object3D>("GraphicsTabItems"));

	// グラフィックス項目のオブジェクト設定
	mGraphicsUIObj[POSTPROCESS].planeObj = SceneManager::FindObject<Object3D>("PostProcess");
	mGraphicsUIObj[SHADOW].planeObj = SceneManager::FindObject<Object3D>("Shadow");
}

void GraphicsTab::Update()
{

	mGraphicsUIObj[mCurrentNum].state = SELECT;

	if (Input::Key::Triggered(DIK_W))
	{
		mCurrentNum--;
		if (mCurrentNum <= 0)
		{
			mCurrentNum = 0;
		}
		mGraphicsUIObj[mCurrentNum].state = SELECT;
	}
	if (Input::Key::Triggered(DIK_S))
	{
		mCurrentNum++;
		if (mCurrentNum >= mGraphicsOptionNum - 1)
		{
			mCurrentNum = mGraphicsOptionNum - 1;
		}
		mGraphicsUIObj[mCurrentNum].state = SELECT;
	}

	if (Input::Key::Triggered(DIK_SPACE))
	{
		mGraphicsFlags[mCurrentNum] = !mGraphicsFlags[mCurrentNum];
	}

	// それぞれの項目が選ばれているかどうか
	for (size_t i = 0; i < mGraphicsOptionNum; i++)
	{
		if (i != mCurrentNum)
		{
			mGraphicsUIObj[i].state = DISABLED;
		}

		switch (mGraphicsUIObj[i].state)
		{
		case DISABLED:
			mGraphicsUIObj[i].planeObj->scale = mGraphicsNoneSelectScale;
			break;
		case SELECT:
			mGraphicsUIObj[i].planeObj->scale = mGraphicsSelectScale;
			break;
		}

		mGraphicsUIObj[i].planeObj->Update();
	}

	int flagValue = mGraphicsFlags[mCurrentNum];
	ConsoleWindow::Log(std::format("サウンドアイテム値:{}:{}\n", mCurrentNum, flagValue));
}

void GraphicsTab::Draw()
{
}

void GraphicsTab::OnUpdate()
{
	mItemsParentObj->Activate();
	
	mCurrentNum = 0;
}

void GraphicsTab::OffUpdate()
{
	mItemsParentObj->Deactivate();

	mCurrentNum = 0;
}
