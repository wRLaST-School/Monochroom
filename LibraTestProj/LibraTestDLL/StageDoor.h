#pragma once
#include "IScriptObject.h"
#include "Object3D.h"
#include "Util/Easing.h"


class StageDoor :
	public IScriptObject
{
private:
	const int kMoveTime = 45;
	bool mIsMoving = false;
	bool mIsClosing = true;

	const Vec3 kClosingPosLTmp = { -1.0f,2.0f,0 };
	const Vec3 kClosingPosRTmp = { 1.0f,2.0f,0 };
	const Vec3 kOpeningPosExtendR = { 2.0f,0,0 };
	const Vec3 kOpeningPosExtendL = { 2.0f,0,0 };

	Vec3 mStartPosL;
	Vec3 mStartPosR;
	Vec3 mEndPosL;
	Vec3 mEndPosR;

	const int kCloseLimitTime = 7;
	int mCloseLimitTime = 0;

	std::unique_ptr<Easing>mEasing;


private:
	void OpClDoorInternal(const Vec3& sPosL, const Vec3& ePosL,
		const Vec3& sPosR, const Vec3& ePosR, bool isClosing);

public:
	void OpenDoor();
	void CloseDoor();

public:
	void Init();
	void Update();
	void Draw();
	void CopyComponent(IComponent* src) { src; }

	DefDel;
};

RegisterScript(StageDoor);

