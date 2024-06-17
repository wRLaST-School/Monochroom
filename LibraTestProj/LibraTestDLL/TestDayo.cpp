#include "TestDayo.h"
#include <ScriptComponent.h>
#include <SpMath.h>
#include <Object3D.h>

void TestDayo::Init()
{

}

void TestDayo::Update()
{
	auto obj = This()->Parent()->CastTo<Object3D>();
	obj->rotationE.y += ConvertAngleToRadian(10);
}

void TestDayo::Draw()
{

}

void TestDayo::CopyComponent(IComponent* src)
{

}
