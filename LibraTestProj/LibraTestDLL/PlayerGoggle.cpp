#include "PlayerGoggle.h"
#include <ConsoleWindow.h>
#include <ConsoleWindow.h>
#include <AppOperationCommand.h>
#include <GoggleScr.h>
#include <SceneManager.h>
#include <SpriteObject.h>
#include <GrayScale.h>
#include <PlayerGoggleState.h>


void PlayerGoggle::Init()
{
	ChangeState(std::make_unique<PlayerGoggleStateNone>());
}

void PlayerGoggle::Update()
{
	mState->Update();
}

void PlayerGoggle::Draw()
{
}


//-----------------------------------------------------------------------------------------
void PlayerGoggle::GettedGoggle()
{
	ChangeState(std::make_unique<PlayerGoggleStateHaving>());
}

void PlayerGoggle::TakeOnOffGoggle(bool isOn)
{
	auto goggleObj = SceneManager::FindObject<Object3D>("Goggle");
	auto goggleScr = SceneManager::FindChildObject<GoggleScr>("GoggleScr", goggleObj);
	//着脱
	goggleScr->TakeOnOff(isOn);
}

void PlayerGoggle::AlignGoggle2DPos()
{
	//ゴーグルのモデルと画像の方の位置を合わせるため
	GrayScale::info.offsetRatio = GetGoggleScr()->GetPosRatio();
}

void PlayerGoggle::FinishedEquip()
{
	auto flyBlocks = SceneManager::FindObjectsWithTag<Object3D>("FlyBlock");
	auto glasses = SceneManager::FindObjectsWithTag<Object3D>("Glass");

	for (auto& fb : flyBlocks)
	{
		fb->useSilhouette = true;
	}
	for (auto& glass : glasses)
	{
		glass->Deactivate();
	}

	//グレースケールの演出
	GrayScale::BeginGrayEffect(true, kEquipGrayEffectTime);
}

void PlayerGoggle::FinishedTakeOff()
{
	auto flyBlocks = SceneManager::FindObjectsWithTag<Object3D>("FlyBlock");
	auto glasses = SceneManager::FindObjectsWithTag<Object3D>("Glass");

	for (auto& fb : flyBlocks)
	{
		fb->useSilhouette = false;
	}
	for (auto& glass : glasses)
	{
		glass->Activate();
	}
}

void PlayerGoggle::TakeOffGrayScaleEffect()
{
	//グレースケールの演出
	GrayScale::BeginGrayEffect(false, kTakeOffGrayEffectTime);
}

bool PlayerGoggle::GetFinishedEquipGoggle()
{
	bool result = GetGoggleScr()->GetIsEquip() && !GetGoggleScr()->GetIsMoving();

	return result;
}

bool PlayerGoggle::GetFinishedTakingOffGoggle()
{
	bool result = GetGoggleScr()->GetIsMoving() && !GetGoggleScr()->GetIsEquip();

	return result;
}

bool PlayerGoggle::GetIsFinishedGrayScaleEffect()
{
	bool result = !GrayScale::GetIsEffect();

	return result;
}

//-----------------------------------------------------------------------------------------
GoggleScr* PlayerGoggle::GetGoggleScr()
{
	auto goggleObj = SceneManager::FindObject<Object3D>("Goggle");
	auto goggleScr = SceneManager::FindChildObject<GoggleScr>("GoggleScr", goggleObj);

	return goggleScr;
}

void PlayerGoggle::ChangeState(std::unique_ptr<IPlayerGoggleState> state)
{
	mState.reset();
	mState = std::move(state);

	mState->SetPlayerGoggleClass(this);
	mState->Init();
}

RegisterScriptBody(PlayerGoggle);
