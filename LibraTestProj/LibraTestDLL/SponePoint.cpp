#include "SponePoint.h"
#include <ScriptComponent.h>
#include <SceneManager.h>
#include "AppOperationCommand.h"


void SponePoint::Init()
{
	mParent = This()->Parent()->CastTo<Object3D>();

	SponePlayer();
}

void SponePoint::Update()
{
	if (AppOperationCommand::GetInstance()->ReStartCommand())
	{
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
