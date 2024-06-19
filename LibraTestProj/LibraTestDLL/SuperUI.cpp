#include "SuperUI.h"
#include <Camera.h>


void SuperUI::Init()
{
	// 現在のカメラ座標を代入
	mUIParentObj.position = Camera::sCurrent->position;

}

void SuperUI::Update()
{

}

void SuperUI::Draw()
{

}

void SuperUI::CopyComponent(IComponent* src)
{

}
