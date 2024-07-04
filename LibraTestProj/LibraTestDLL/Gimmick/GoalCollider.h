#pragma once
#include "IScriptObject.h"
#include <SphereCollider.h>
#include <Object3D.h>

class GoalCollider :
	public IScriptObject
{
private:
	Object3D* mObj;
	SphereCollider mClearCollider;

public:
	void Init();
	void Update();
	void Draw();
	void CopyComponent(IComponent* src) { src; }

	DefDel;
};

RegisterScript(GoalCollider);

