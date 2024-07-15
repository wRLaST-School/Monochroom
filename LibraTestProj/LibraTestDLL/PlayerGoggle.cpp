#include "PlayerGoggle.h"
#include <ConsoleWindow.h>
#include <ConsoleWindow.h>
#include <AppOperationCommand.h>
#include <GoggleScr.h>
#include <SceneManager.h>
#include <SpriteObject.h>


void PlayerGoggle::Init()
{
	mIsEquipGoggle = false;
	mIsHavingGoggle = false;
}

void PlayerGoggle::Update()
{
	if (mIsHavingGoggle)
	{
		if (mIsEquipGoggle)
		{
			ConsoleWindow::Log("EquipGoggle");
		}

		ConsoleWindow::Log("GetGoggle");

		auto goggleObj = SceneManager::FindObject<Object3D>("Goggle");
		auto goggleScr = SceneManager::FindChildObject<GoggleScr>("GoggleScr", goggleObj);


		//ゴーグルの着脱
		if (AppOperationCommand::GetInstance()->PlayerEquipGoggleCommand())
		{
			ConsoleWindow::Log("ChangeEquip");

			mIsEquipGoggle = !mIsEquipGoggle;

			auto glayScale = SceneManager::FindObject<IComponent>("GrayScale");
			auto flyBlocks = SceneManager::FindObjectsWithTag<Object3D>("FlyBlock");
			auto glasses = SceneManager::FindObjectsWithTag<Object3D>("Glass");

			//装着後か外した後か
			if (mIsEquipGoggle)
			{
				glayScale->Activate();

				for (auto& fb : flyBlocks)
				{
					fb->useSilhouette = true;
				}
				for (auto& glass : glasses)
				{
					glass->Deactivate();
				}
			}
			else
			{
				glayScale->Deactivate();

				for (auto& fb : flyBlocks)
				{
					fb->useSilhouette = false;
				}
				for (auto& glass : glasses)
				{
					glass->Activate();
				}
			}

			//着脱
			goggleScr->TakeOnOff(mIsEquipGoggle);
		}
	}
}

void PlayerGoggle::Draw()
{

}

void PlayerGoggle::SetIsHavingGoggle(bool isHaving)
{
	mIsHavingGoggle = isHaving;
}

RegisterScriptBody(PlayerGoggle);
