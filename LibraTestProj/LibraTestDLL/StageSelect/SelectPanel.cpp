#include "SelectPanel.h"
#include <ScriptComponent.h>
#include <SpDS.h>
#include <Util.h>
#include <Input.h>
#include <SceneManager.h>
#include <ConsoleWindow.h>

void SelectPanel::Init()
{
	mSelectMaxNum = { 3,2 };
	mSelectCurrentNum = { 0,0 };

	mStageNumObj.resize((int)mSelectMaxNum.y);
	mStageNumState.resize((int)mSelectMaxNum.y);

	mDisabledPanelScale = { 1,1,1 };
	mSelectPanelScale = {1.1f,1.1f,1};
	mPressedPanelScale = {1,1,1};


	for (size_t i = 0; i < mSelectMaxNum.y; i++)
	{
		mStageNumObj[i].resize((int)mSelectMaxNum.x);
		mStageNumState[i].resize((int)mSelectMaxNum.x);
	}

	for (size_t i = 0; i < mSelectMaxNum.y; i++)
	{
		for (size_t j = 0; j < mSelectMaxNum.x; j++)
		{
			std::string stageFirstName = "Stage";
			std::string stageBaseNum = std::to_string(i + 1) + "-" + std::to_string(j + 1);

			std::string stageName = stageFirstName + stageBaseNum;

			mStageNumObj[i][j] = SceneManager::FindObject<Object3D>(stageName);
			mStageNumState[i][j] = DISABLED;
		}
		
	}
	
}

void SelectPanel::Update()
{
	

	if(Input::Key::Triggered(DIK_A))
	{
		if (mSelectCurrentNum.x > 0) 
		{
			mSelectCurrentNum.x--;
		}
	}
	if (Input::Key::Triggered(DIK_D))
	{
		if (mSelectCurrentNum.x < mSelectMaxNum.x - 1)
		{
			mSelectCurrentNum.x++;
		}
	}

	if (Input::Key::Triggered(DIK_W))
	{
		if (mSelectCurrentNum.y > 0)
		{
			mSelectCurrentNum.y--;
		}
	}
	if (Input::Key::Triggered(DIK_S))
	{
		if (mSelectCurrentNum.y < mSelectMaxNum.y - 1)
		{
			mSelectCurrentNum.y++;
		}
	}

	mStageNumState[(int32_t)mSelectCurrentNum.y][(int32_t)mSelectCurrentNum.x] = SELECT;
	ConsoleWindow::Log(std::format("ステージ現在番号：Y,{} X,{}", mSelectCurrentNum.y, mSelectCurrentNum.x));

	for (size_t i = 0; i <  mSelectMaxNum.y; i++)
	{
		for (size_t j = 0; j < mSelectMaxNum.x; j++)
		{
			if (i != mSelectCurrentNum.y ||
				j != mSelectCurrentNum.x)
			{
				mStageNumState[i][j] = DISABLED;
			}

			switch (mStageNumState[i][j])
			{
			case DISABLED:
				mStageNumObj[i][j]->scale = mDisabledPanelScale;
				break;
			case SELECT:
				mStageNumObj[i][j]->scale = mSelectPanelScale;
				break;
			case PRESSED:
				mStageNumObj[i][j]->scale = mPressedPanelScale;
				break;
			}

			mStageNumObj[i][j]->Update();
		}
	}

	ConsoleWindow::Log("ステージ番号オブジェを更新");
}

void SelectPanel::Draw()
{
}

void SelectPanel::OnInspectorWindowDraw()
{
}

void SelectPanel::CopyComponent(IComponent* src)
{
}

RegisterScriptBody(SelectPanel);