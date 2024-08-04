#include "GameManager.h"
#include <SceneFromFile.h>
#include <SceneManager.h>
#include <ConsoleWindow.h>
#include <AppOperationCommand.h>
#include <StageGenerating.h>
#include <BlinkTransition.h>
#include <Input.h>
#include <FlyBlockWhite.h>
#include <GoggleScr.h>
#include <PlayerGoggle.h>
#include <GrayScale.h>
#include <SpriteObject.h>

void GameManager::Awake()
{
	mPlayer = SceneManager::FindObject<Object3D>("Player");
	mCamera = SceneManager::FindObject<Camera>("Camera");
	mStageGenerater = SceneManager::FindObjectWithTag<StageGenerater>("StageGenerater");
	mSelectPanel = SceneManager::FindObject<SelectPanel>("SelectScript");
	mUIScript = SceneManager::FindObject<SuperUI>("UIScript");
	mAttractParticleManager = SceneManager::FindObjectWithTag<AttractParticleManager>("AttractParticleManager");
	isCantControl = false;

	//プレイヤーがすでにゴーグル取ってたら
	if (GrayScale::mIsHavingGoggle)
	{
		auto goggle = SceneManager::FindObject<Object3D>("Goggle");
		if (goggle)
		{
			auto goggleScr = SceneManager::FindChildObject<GoggleScr>("GoggleScr", goggle);
			if (goggleScr)
				PlayerGettedGoggle(mPlayer, goggleScr);
		}
	}

	auto grayScale = SceneManager::FindObject<SpriteObject>("GrayScale");
	if (grayScale)
	{
		grayScale->SetPosition({ 960.0f,500.0f });
	}

	isStop = false;

	if (SceneManager::GetCurrentScene()->GetName() == "Title" ||
		SceneManager::GetCurrentScene()->GetName() == "StageSelect")
	{
		StageGenerating::info.isDraw = false;
		//BlinkTransition::mIsChangeScene = false;
		BlinkTransition::Reset();
	}
	else
	{
		StageGenerating::info.isDraw = true;
	}
}

void GameManager::Init()
{
}

void GameManager::Update()
{
	if (AppOperationCommand::GetInstance()->UserDebugStopGameCommand())
	{
		isStop = isStop ? false : true;
	}

	//ConsoleWindow::Log(std::format("GameManager::isStop : {}", isStop));

	if (SceneManager::GetCurrentScene()->GetName() == "StageSelect")
	{
		if (BlinkTransition::mIsChangeScene &&
			BlinkTransition::mIsLoaded)
		{
			BlinkTransition::mIsChangeScene = false;
			BlinkTransition::mIsLoaded = false;
			BlinkTransition::Reset();
		}
	}
	else
	{
		if (BlinkTransition::mIsChangeScene &&
			BlinkTransition::mIsLoaded)
		{
			BlinkTransition::TransitionOut();

			if (BlinkTransition::info.isOutEnd)
			{
#ifdef _NDEBUG
				// ステージ生成
				mStageGenerater->Start();
#endif

				BlinkTransition::mIsChangeScene = false;
				BlinkTransition::mIsLoaded = false;
			}
		}
	}

	if (!SceneManager::GetCurrentScene())
	{
		//ConsoleWindow::Log("SceneManager::GetCurrentScene is null");
		return;
	}

	if (SceneManager::GetCurrentScene()->GetName() == "StageSelect")
	{
		//ConsoleWindow::Log("Select Is Set");
		if (!mSelectPanel)
		{
			//ConsoleWindow::Log("mSelectPanel is null");
			return;
		}
		if (mSelectPanel->GetIsChangeScene())
		{
			if (!BlinkTransition::mIsChangeScene)
			{
				BlinkTransition::Start();
				BlinkTransition::mIsChangeScene = true;
				SoundManager::Play("Blink");
			}
		}

		if (BlinkTransition::mIsChangeScene)
		{
			if (BlinkTransition::info.isInEnd)
			{
				// シーンの切り替え処理
				SceneManager::LoadScene<SceneFromFile>(mSelectPanel->GetStageName());
				SceneManager::WaitForLoadAndTransition();
				BlinkTransition::mIsLoaded = true;
			}
		}
	}
	else
	{
		//ConsoleWindow::Log("Game Is Set");
		if (!mUIScript)
		{
			//ConsoleWindow::Log("UI is null");
			return;
		}
		// UIでタイトルにもどる時
		if (mUIScript->GetBackToTitle())
		{
			//OutputDebugStringA("SceneChangeClickToSelectScene\n");

			if (!BlinkTransition::mIsChangeScene)
			{
				BlinkTransition::mToTitle = true;
				BlinkTransition::Start();
				BlinkTransition::mIsChangeScene = true;
			}
		}

		// リセット
		std::string sceneName = SceneManager::GetCurrentScene()->GetTag(0);
		if (AppOperationCommand::GetInstance()->ReStartCommand())
		{
			BlinkTransition::mReset = true;
			BlinkTransition::Start();
			BlinkTransition::mIsChangeScene = true;
		}

		if (BlinkTransition::mIsChangeScene)
		{
			if (BlinkTransition::info.isInEnd)
			{
				if (BlinkTransition::mToTitle)
				{
					// シーンの切り替え処理
					SceneManager::LoadScene<SceneFromFile>("Assets/Scene/StageSelect.scene");
					SceneManager::WaitForLoadAndTransition();
					BlinkTransition::mToTitle = false;
					BlinkTransition::mIsLoaded = true;
				}
				else if (BlinkTransition::mReset)
				{
					// シーンの切り替え処理
					SceneManager::LoadScene<SceneFromFile>("Assets/Scene/Stage/" + sceneName + ".scene");
					SceneManager::WaitForLoadAndTransition();
					BlinkTransition::mReset = false;
					BlinkTransition::mIsLoaded = true;
				}
			}
		}

		if (!mStageGenerater)
		{
			//ConsoleWindow::Log("mStageGenerater is null");
			return;
		}

		if (isStop == false)
		{
			Input::Mouse::SetPos(Float2(960, 540));
			Input::Mouse::HideCursor();
		}
		else
		{
			Input::Mouse::ShowCursorM();
		}
#ifdef _NDEBUG
		mStageGenerater->PositionSetting();
#endif

#ifdef _DEBUG
		// ステージ生成
		mStageGenerater->PositionSetting();
		mStageGenerater->Start();
#endif

	}

	//ConsoleWindow::Log("GameManager: isActive");
	BlinkTransition::TransitionIn();
}

void GameManager::Draw()
{

}

void GameManager::CopyComponent(IComponent* src)
{

}

Object3D* GameManager::GetPlayer()
{
	return mPlayer;
}

Camera* GameManager::GetCamera()
{
	return mCamera;
}

StageGenerater* GameManager::GetStageGenerater()
{
	return mStageGenerater;
}

AttractParticleManager* GameManager::GetAttractParticleManager()
{
	return mAttractParticleManager;
}

FlyBlock* GameManager::GetFlyBlock(IComponent* parentComp)
{
	auto fb = SceneManager::FindChildObject<FlyBlock>("FlyBlock", parentComp);

	if (fb == nullptr)
	{
		fb = SceneManager::FindChildObject<FlyBlockWhite>("FlyBlockWhite", parentComp);
	}

	return fb;
}

void GameManager::PlayerGettedGoggle(Object3D* player, GoggleScr* goggleScr)
{
	if (goggleScr && player)
	{
		auto grayScale = SceneManager::FindObject<SpriteObject>("GrayScale");
		if (grayScale)
		{
			grayScale->Activate();
			//grayScale->SetPosition({ 960.0f,500.0f });
		}
		GrayScale::EffectInit();

		goggleScr->GettedPlayer(player);

		auto playerGoggle = SceneManager::FindChildObject<PlayerGoggle>("PlayerGoggle", player);
		if (playerGoggle)
			playerGoggle->GettedGoggle();
	}
}

bool GameManager::GetisStop()
{
	return isStop;
}

void GameManager::SetIsStop(bool IsStop)
{
	isStop = IsStop;
}

GameManager* GameManager::GetInstance()
{
	GameManager* instance = SceneManager::FindObject<GameManager>("GameManager");
	return instance;
}

RegisterScriptBody(GameManager);