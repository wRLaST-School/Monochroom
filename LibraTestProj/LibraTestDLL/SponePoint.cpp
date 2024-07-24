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

	if (GameManager::GetInstance()->GetisStop() &&
		!GameManager::GetInstance()->GetStageGenerater()->GetisEnd())
	{
		ConsoleWindow::Log("SPONE_PLAYER");
		SponePlayer();
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
