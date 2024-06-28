#pragma once
#include "IScriptObject.h"
#include <Object3D.h>
#include <Util/Easing.h>
#include <Gravity.h>


class FlyBlock :
	public IScriptObject
{
private:
	Object3D* mParent = nullptr;
	std::unique_ptr<Gravity> mGravity = nullptr;

	bool mIsAttracted = false;
	Vec3 mBeginPos;
	Vec3 mEndPos;

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

	DefDel;
};

RegisterScript(FlyBlock);

