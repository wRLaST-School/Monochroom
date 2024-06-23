#include "GameManager.h"
#include "SceneManager.h"

void GameManager::Init()
{
	mPlayer = SceneManager::FindObject<Object3D>("Player");
}

void GameManager::Update()
{

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
