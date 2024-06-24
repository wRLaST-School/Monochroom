#include "GameManager.h"
#include <SceneManager.h>
#include <ConsoleWindow.h>

void GameManager::Init()
{
	mPlayer = SceneManager::FindObject<Object3D>("Player");
	if (!ConsoleWindow::NullCheck<Object3D>(mPlayer))
	{
		ConsoleWindow::Log("mPlayer is Null");
		return;
	}
}

void GameManager::Update()
{
	if (!ConsoleWindow::NullCheck<Object3D>(mPlayer))
	{
		ConsoleWindow::Log("mPlayer is Null");
		return;
	}
	ConsoleWindow::Log("mPlayer is Not Null");
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

GameManager* GameManager::GetInstance()
{
	static GameManager instance;
	return &instance;
}

RegisterScriptBody(GameManager);