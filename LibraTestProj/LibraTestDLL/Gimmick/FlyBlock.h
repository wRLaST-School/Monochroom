#pragma once
#include "IScriptObject.h"
#include <Object3D.h>
#include <Util/Easing.h>
#include <Gravity.h>


class FlyBlock :
	public IScriptObject
{
public:
	enum BlockType
	{
		NORMAL,
		WHITE,
	};

protected:
	BlockType type;

public:
	static const float skAttractedHittingNotEndDot;

protected:
	Object3D* mParent = nullptr;
	std::unique_ptr<Gravity> mGravity = nullptr;

	bool mIsUpdate = true;

	Vec3 mResponePos = { 0,0,0 };
	Vec3 mResponeRot = { 0,0,0 };

	const Vec3 kResponePos = { 0,-30.0f,0 };

	bool mIsAttracted = false;
	Vec3 mBeginPos;
	Vec3 mEndPos;

	std::unique_ptr<Easing> mEasing = nullptr;

	const int16_t kAttractedFrameMax = 60 * 2;

	//自分の下にいるブロックの引き寄せのべくとるとかで使う
	Vec3 mAttractParentVec = { 0,0,0 };

	Vec3 mOldAttractPos = { 0,0,0 };

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
	Vec3 GetBeginPos() { return mBeginPos; }
	Vec3 GetEndPos() { return mEndPos; }

	void ZeroGravity() { mGravity->ZeroVelocity(); }
	Gravity* GetGravity() { return mGravity.get(); }

	void SetIsUpdate(bool is) { mIsUpdate = is; }
	bool GetIsUpdate() { return mIsUpdate; }

public:
	void SetAttractVec(const Vec3& vec) { mAttractParentVec = vec; }
	const Vec3& GetAttractVec() { return mAttractParentVec; }

	DefDel;
};

RegisterScript(FlyBlock);

