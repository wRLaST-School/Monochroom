#pragma once
#include "IScriptObject.h"
#include "Bases.h"
#include <OBBCollider.h>
#include <SphereCollider.h>
class TestObject :
	public IScriptObject
{
private:
	//OBBCollider obbCollider;
	//SphereCollider sphereCollider;
	//int hoge[25000000] = {};

public:
	void Init();
	void Update();
	void Draw();
	void OnInspectorWindowDraw() override;
	void CopyComponent(IComponent* src);
	DefDel;
};

RegisterScript(TestObject);