#include "StageGoal.h"
#include <ScriptComponent.h>
#include <SceneManager.h>
#include <SceneFromFile.h>
#include <ConsoleWindow.h>
#include <Input.h>

void StageGoal::Init()
{
	mObj = This()->Parent()->CastTo<Object3D>();
	clearState = None;
}

void StageGoal::Update()
{
	if (clearState == ChangeScene)
	{
		std::string directory = "Assets/Scene/Stage/";
		std::string sceneName = This()->GetTag(0);
		std::string ext = ".scene";
		std::string fullpath = directory + sceneName + ext;

		// シーンの切り替え処理
		SceneManager::LoadScene<SceneFromFile>(fullpath);
		SceneManager::WaitForLoadAndTransition();
		ConsoleWindow::Log("FullPath : " + fullpath);

		clearState = Clear;
	}
}

void StageGoal::Draw()
{

}

RegisterScriptBody(StageGoal);
