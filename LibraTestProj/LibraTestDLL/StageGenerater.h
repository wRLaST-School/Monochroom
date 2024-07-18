#pragma once
#include "IScriptObject.h"
#include <Object3D.h>

class StageGenerater :
	public IScriptObject
{
private:
	bool mIsStart;
	bool mIsStarted;
	bool mIsEnd;
	Object3D* mObj;
	Object3D* mPlaneAlpha;
	Object3D* mPlane;
	Vec3 mMoveVec;

public:
	void Init();
	void Update();
	void Draw();
	void CopyComponent(IComponent* src) { src; }

	DefDel;

public:
	void Start();
	bool GetisEnd();
};

RegisterScript(StageGenerater);

