#pragma once
#include "IScriptObject.h"
#include <Object3D.h>
#include <Easing.h>

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
	Object3D* mStageObj;
	Vec3 mMoveVec;
	float mMoveSpeed;
	Easing mEase;

	Vec3 mEnd;
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

