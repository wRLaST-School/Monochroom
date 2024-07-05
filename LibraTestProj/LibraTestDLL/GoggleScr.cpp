#include "GoggleScr.h"
#include <ScriptComponent.h>
#include <ConsoleWindow.h>


void GoggleScr::Init()
{
	mParent = This()->Parent()->CastTo<Object3D>();
}

void GoggleScr::Update()
{
	if (mOwner)
	{
		Vec3 pos = Vec3{ mOwner->position.x,mOwner->position.y,mOwner->position.z } + mLocalPos;
		mParent->position = { pos.x,pos.y,pos.z };

		mParent->rotationE = mOwner->rotationE;
	}
}

void GoggleScr::Draw()
{

}

void GoggleScr::GettedPlayer(Object3D* owner)
{
	mOwner = owner;

	ConsoleWindow::Log("GoggleGet!!!!!!!!!!");
}

RegisterScriptBody(GoggleScr);
