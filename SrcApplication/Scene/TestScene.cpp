#include "TestScene.h"
#include "RTVManager.h"
#include "SpSwapChainManager.h"
#include "Input.h"
#include "RayCollider.h"
#include "TextDrawer.h"
#include <LineDrawer.h>
#include <SceneFromFile.h>
#include <SceneManager.h>

void TestScene::LoadResources()
{

}

void TestScene::Init()
{
	Camera* cam = this->AddComponent<Camera>("Camera");
	cam->UseDefaultParams();

	Camera::Set(*cam);
}

void TestScene::Update()
{
#ifndef _DEBUG
	SceneManager::LoadScene<SceneFromFile>("Assets/Scene/StageSelect.scene");
	SceneManager::WaitForLoadAndTransition();
#endif
}

void TestScene::DrawBack()
{
}

void TestScene::Draw3D()
{
}

void TestScene::DrawSprite()
{
	
}
