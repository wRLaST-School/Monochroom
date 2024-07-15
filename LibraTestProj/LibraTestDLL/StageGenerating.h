#pragma once
#include "IScriptObject.h"
#include <Object3D.h>

class StageGenerating :
	public IScriptObject
{
private:
	bool isStart;
	Object3D* mObj;
	Object3D* mPlaneAlpha;
	Object3D* mPlane;

public:
	void Init();
	void Update();
	void Draw();
	void CopyComponent(IComponent* src) { src; }

	DefDel;
};

RegisterScript(StageGenerating);

