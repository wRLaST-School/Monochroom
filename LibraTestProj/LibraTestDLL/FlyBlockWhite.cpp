#include "FlyBlockWhite.h"

void FlyBlockWhite::Init()
{
	FlyBlock::Init();

	type = BlockType::WHITE;

	Parent()->CastTo<Object3D>()->Object3D::texType = 2;
}

void FlyBlockWhite::Update()
{
	FlyBlock::Update();

	//if (!mIsUpdate)
	//{
	//	EndAttracting();
	//}
}

void FlyBlockWhite::Draw()
{

}

RegisterScriptBody(FlyBlockWhite);
