#include "GraphicsTab.h"
#include <Camera.h>
#include <SpDS.h>
#include <SpTextureManager.h>
#include <Util.h>
#include <SceneManager.h>
#include <Input.h>
#include <ConsoleWindow.h>
#include "GameManager.h"
#include "AppOperationCommand.h"

void GraphicsTab::Init()
{
	//mGraphicsOptionNum = 2;

	//mGraphicsFlags.resize(mGraphicsOptionNum);

	//mGraphicsNoneSelectScale = { 0.9f,0.6f,1 };
	//mGraphicsSelectScale = { 1,0.8f,1 };

	//mGraphicsDefuValue = 1;
	//mGraphicsMaxValue = 10;

	//mGraphicsUIObj.resize(mGraphicsOptionNum);


	//mItemsParentObj = SceneManager::FindObject<Object3D>("GraphicsTabItems");

	//// グラフィックス項目のオブジェクト設定
	//mGraphicsUIObj[POSTPROCESS].planeObj = SceneManager::FindObject<Object3D>("PostProcess");
	//mGraphicsUIObj[SHADOW].planeObj = SceneManager::FindObject<Object3D>("Shadow");

	//// オンオフオブジェクト
	//mOnPlaneObj.resize(mGraphicsOptionNum);
	//mOffPlaneObj.resize(mGraphicsOptionNum);

	//for (size_t i = 0; i < mGraphicsOptionNum; i++)
	//{
	//	mOnPlaneObj[i] = SceneManager::FindChildObject<Object3D>("OnPlane", mGraphicsUIObj[i].planeObj);
	//	mOffPlaneObj[i] = SceneManager::FindChildObject<Object3D>("OffPlane", mGraphicsUIObj[i].planeObj);
	//	mGraphicsFlags[i] = true;
	//}

	//mSelectColor = { 1.0f,1.0f,1.0f,1.0f };
	//mDisabledColor = { 0.8f,0.8f,0.8f,1.0f };

	//mGraphicsCurrentNum = 0;
}

void GraphicsTab::Update()
{


}

void GraphicsTab::Draw()
{
}

void GraphicsTab::CopyComponent(IComponent* src)
{
}

void GraphicsTab::MenuUpdate()
{
	//mGraphicsUIObj[mGraphicsCurrentNum].state = SELECT;

	//if (AppOperationCommand::GetInstance()->UISelectUpCommand())
	//{
	//	mGraphicsCurrentNum--;
	//	if (mGraphicsCurrentNum <= 0)
	//	{
	//		mGraphicsCurrentNum = 0;
	//	}
	//	mGraphicsUIObj[mGraphicsCurrentNum].state = SELECT;
	//}
	//if (AppOperationCommand::GetInstance()->UISelectDownCommand())
	//{
	//	mGraphicsCurrentNum++;
	//	if (mGraphicsCurrentNum >= mGraphicsOptionNum - 1)
	//	{
	//		mGraphicsCurrentNum = mGraphicsOptionNum - 1;
	//	}
	//	mGraphicsUIObj[mGraphicsCurrentNum].state = SELECT;
	//}

	//// フラグのオンオフ
	//if (AppOperationCommand::GetInstance()->UISelectLeftCommand())
	//{
	//	mGraphicsFlags[mGraphicsCurrentNum] = true;


	//}

	//if (AppOperationCommand::GetInstance()->UISelectRightCommand())
	//{
	//	mGraphicsFlags[mGraphicsCurrentNum] = false;

	//}

	//// それぞれの項目が選ばれているかどうか
	//for (size_t i = 0; i < mGraphicsOptionNum; i++)
	//{
	//	if (i != mGraphicsCurrentNum)
	//	{
	//		mGraphicsUIObj[i].state = DISABLED;
	//	}

	//	switch (mGraphicsUIObj[i].state)
	//	{
	//	case DISABLED:
	//		mGraphicsUIObj[i].planeObj->scale = mGraphicsNoneSelectScale;
	//		break;
	//	case SELECT:
	//		mGraphicsUIObj[i].planeObj->scale = mGraphicsSelectScale;
	//		break;
	//	}

	//	if (mGraphicsFlags[i])
	//	{
	//		*mOnPlaneObj[i]->brightnessCB.contents = mSelectColor.f4;
	//		*mOffPlaneObj[i]->brightnessCB.contents = mDisabledColor.f4;
	//	}
	//	else
	//	{
	//		*mOnPlaneObj[i]->brightnessCB.contents = mDisabledColor.f4;
	//		*mOffPlaneObj[i]->brightnessCB.contents = mSelectColor.f4;
	//	}
	//	mOnPlaneObj[i]->Update();
	//	mOffPlaneObj[i]->Update();

	//	mGraphicsUIObj[i].planeObj->Update();
	//}

	//int flagValue = mGraphicsFlags[mGraphicsCurrentNum];
	//ConsoleWindow::Log(std::format("グラフィックアイテム値:{}:{}\n", mGraphicsCurrentNum, flagValue));
}

void GraphicsTab::OnUpdate()
{
	mItemsParentObj->Activate();
	mGraphicsCurrentNum = 0;
	MenuUpdate();
}

void GraphicsTab::OffUpdate()
{
	mItemsParentObj->Deactivate();
}
RegisterScriptBody(GraphicsTab);