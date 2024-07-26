#include "PlayerGoggleState.h"
#include <GrayScale.h>
#include <AppOperationCommand.h>
#include <PlayerGoggle.h>
#include <SceneManager.h>


//ゴーグル装着していない-------------------------------------------
void PlayerGoggleStateNone::Init()
{

}

void PlayerGoggleStateNone::Update()
{

}

//ゴーグル持ってる-------------------------------------------------
void PlayerGoggleStateHaving::Init()
{
}

void PlayerGoggleStateHaving::Update()
{
	//画像の方のゴーグルの位置合わせ
	mPlayerGoggle->AlignGoggle2DPos();

	//装着のキー押したら
	if (AppOperationCommand::GetInstance()->PlayerEquipGoggleCommand())
	{
		mPlayerGoggle->ChangeState(std::make_unique<PlayerGoggleStateEquiping>());
	}
}

//ゴーグル装着途中------------------------------------------------
void PlayerGoggleStateEquiping::Init()
{
	GrayScale::info.isDraw = true;
	//装着はじめ
	mPlayerGoggle->TakeOnOffGoggle(true);
}

void PlayerGoggleStateEquiping::Update()
{
	//画像の方のゴーグルの位置合わせ
	mPlayerGoggle->AlignGoggle2DPos();

	//装着完了したら
	if (mPlayerGoggle->GetFinishedEquipGoggle())
	{
		mPlayerGoggle->ChangeState(std::make_unique<PlayerGoggleStateEquiped>());
	}
}

//ゴーグル装着完了---------------------------------------------------
void PlayerGoggleStateEquiped::Init()
{
	mPlayerGoggle->FinishedEquip();
}

void PlayerGoggleStateEquiped::Update()
{
	if (AppOperationCommand::GetInstance()->PlayerEquipGoggleCommand())
	{
		mPlayerGoggle->ChangeState(std::make_unique<PlayerGoggleStateTakingOffEffect>());
	}
}

//ゴーグル演出途中--------------------------------------------------
void PlayerGoggleStateTakingOffEffect::Init()
{
	mPlayerGoggle->TakeOffGrayScaleEffect();
}

void PlayerGoggleStateTakingOffEffect::Update()
{
	if (mPlayerGoggle->GetIsFinishedGrayScaleEffect())
	{
		mPlayerGoggle->ChangeState(std::make_unique<PlayerGoggleStateTakingOff>());
	}
}

//ゴーグル外し途中--------------------------------------------------
void PlayerGoggleStateTakingOff::Init()
{
	mPlayerGoggle->TakeOnOffGoggle(false);

	mPlayerGoggle->FinishedTakeOff();
}

void PlayerGoggleStateTakingOff::Update()
{
	//画像の方のゴーグルの位置合わせ
	mPlayerGoggle->AlignGoggle2DPos();

	//外し終わったら
	if (mPlayerGoggle->GetFinishedTakingOffGoggle())
	{
		mPlayerGoggle->ChangeState(std::make_unique<PlayerGoggleStateHaving>());
	}
}