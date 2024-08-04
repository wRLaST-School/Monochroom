#include "SponePoint.h"
#include <ScriptComponent.h>
#include <SceneManager.h>
#include <SceneFromFile.h>
#include <IComponent.h>
#include <GameManager.h>
#include <ConsoleWindow.h>
#include "AppOperationCommand.h"

void SponePoint::Init()
{
	mParent = This()->Parent()->CastTo<Object3D>();

	SponePlayer();

	mEasing = std::make_unique<Easing>();
	mEasing->SetEaseTimer(kErasedTimeMax);
	mEasing->SetPowNum(1.5f);
}

void SponePoint::Update()
{
	//if (AppOperationCommand::GetInstance()->ReStartCommand())
	//{
	//	//IComponent::InitAllChildComponents(SceneManager::GetCurrentScene());
	//	// シーンの切り替え処理
	//	std::string sceneName = SceneManager::currentScene->GetName();
	//	SceneManager::LoadScene<SceneFromFile>("Assets/Scene/" + sceneName + ".scene");
	//	SceneManager::WaitForLoadAndTransition();

	//	//SponePlayer();
	//}

	auto player = SceneManager::FindObject<Object3D>("Player");

	if ((GameManager::GetInstance()->GetisStop() &&
		!GameManager::GetInstance()->GetStageGenerater()->GetisEnd()) ||
		player->position.y <= kResponeYMin)
	{
		ConsoleWindow::Log("SPONE_PLAYER");
		SponePlayer();
	}


	if (mEasing->GetTimeRate() < 1.0f)
	{
		mEasing->Update();

		Parent()->CastTo<Object3D>()->scale = mEasing->InBack({ 1.0f,1.0f,1.0f }, { 0,0,0 });
	}
}

void SponePoint::Draw()
{

}


//------------------------------------------------------------
void SponePoint::SponePlayer()
{
	auto player = SceneManager::FindObject<Object3D>("Player");

	player->position = mParent->position;
	player->rotationE = mParent->rotationE;
}

RegisterScriptBody(SponePoint);
