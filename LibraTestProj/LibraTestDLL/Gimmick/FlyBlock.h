#pragma once
#include "IScriptObject.h"
#include <Object3D.h>
#include <Util/Easing.h>
#include <Gravity.h>


class FlyBlock :
	public IScriptObject
{
public:
	static const float skAttractedHittingNotEndDot;

private:
	Object3D* mParent = nullptr;
	std::unique_ptr<Gravity> mGravity = nullptr;

	bool mIsAttracted = false;
	Vec3 mBeginPos;
	Vec3 mEndPos;
	Vec3 mMoveVec;

	std::unique_ptr<Easing> mEasing = nullptr;

	const int16_t kAttractedFrameMax = 60 * 2;

public:
	void Init();
	void Update();
	void Draw();
	void CopyComponent(IComponent* src) { src; }

public:
	void BeginAttracting(const Vec3& endPos);
	void EndAttracting();

	bool GetIsAttracting() { return mIsAttracted; }

	Vec3 GetAttractedDir() { return (mEndPos - mBeginPos).GetNorm(); }

	void ZeroGravity() { mGravity->ZeroVelocity(); }
	Vec3 GetMoveVec() { return mMoveVec; }

	DefDel;
};

RegisterScript(FlyBlock);

