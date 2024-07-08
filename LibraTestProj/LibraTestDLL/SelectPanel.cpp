#include "SelectPanel.h"
#include <SpDS.h>
#include <Util.h>
#include <Input.h>
#include <SceneManager.h>
#include <ConsoleWindow.h>

void SelectPanel::Init()
{
	mSelectMaxNum = { 3,2 };
	
	mStageNumObj.resize((int)mSelectMaxNum.x);

	for (size_t i = 0; i < mSelectMaxNum.y; i++)
	{
		std::string stageParentName = "Stage" + (i + 1);

		Object3D* stageParent = SceneManager::FindObject<Object3D>("Stage1");

		for (size_t j = 0; j < mSelectMaxNum.x; j++)
		{
			mStageNumObj[i].resize((int)mSelectMaxNum.y);

			std::string stageFirstName = "Stage";
			std::string stageBaseNum = (i + 1) + "-" + (j + 1);

			std::string stageName = stageFirstName + stageBaseNum;

			mStageNumObj[i][j] = SceneManager::FindChildObject<Object3D>(stageName, stageParent);
		}
		
	}
	
}

void SelectPanel::Update()
{


	for (size_t i = 0; i <  mSelectMaxNum.y; i++)
	{
		for (size_t j = 0; j < mSelectMaxNum.x; j++)
		{
			mStageNumObj[i][j]->rotationE.y += 0.01f;
			mStageNumObj[i][j]->Update();
			ConsoleWindow::Log("ステージ番号オブジェを更新");
		}
	}
}

void SelectPanel::Draw()
{
}

void SelectPanel::OnInspectorWindowDraw()
{
}
