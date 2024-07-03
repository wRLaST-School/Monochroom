#pragma once
#include "IScriptObject.h"
#include "Object3D.h"
#include "Util/Easing.h"


class StageButton :
	public IScriptObject
{
private:
	IComponent* mParent = nullptr;
	const Vec3 kNormalBodyPos = { 0,0,0 };
	const Vec3 kPushedBodyPos = { 0,-0.61f,0 };
	const int kPushingFrame = 25;
	Vec3 mStartBodyPos;
	Vec3 mEndBodyPos;
	bool mIsPushedBody = false;
	bool mIsMovingBody = false;

	const int kPushBackLimit = 7;
	int mPushBackTimer = 0;

	std::unique_ptr<Easing> mEasing = nullptr;


public:
	void Init();
	void Update();
	void Draw();
	void CopyComponent(IComponent* src) { src; }

private:
	void BeginPushingButtonInter(const Vec3& sPos, const Vec3& ePos);
public:
	void BeginPushingButton();
	void EndPushingButton();

	DefDel;
};

RegisterScript(StageButton);

