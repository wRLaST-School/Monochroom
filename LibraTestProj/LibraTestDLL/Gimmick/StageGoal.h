#pragma once
#include "IScriptObject.h"
#include <Object3D.h>

class StageGoal :
	public IScriptObject
{
public:
	enum ClearState
	{
		None,
		ChangeScene,
		Clear,
	} clearState;

private:
	Object3D* mObj;

public:
	void Init();
	void Update();
	void Draw();
	void CopyComponent(IComponent* src) { src; }

	DefDel;
};

RegisterScript(StageGoal);

