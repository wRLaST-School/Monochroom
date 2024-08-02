#pragma once
#include "IScriptObject.h"
#include <Object3D.h>
#include <Gravity.h>
#include <GameManager.h>
#include <memory>
#include "CameraTab.h"

class PlayerControl :
	public IScriptObject
{
private:
	std::unique_ptr<Gravity> mGravity;
	GameManager* mGameManager;

	const Vec3 JUMP_POWER = { 0,0.38f,0 };

	bool isJump_ = false;
	bool mIsPrepairingCamera = false;
	Vec3 moveVec_ = { 0,0,0 };
	const float MOVE_SPEED = 0.025f;
	const float MOVE_SPEED_MAX = 0.75f;
	const float MOVE_ATTEN = 0.87f;

	const float ANGLE_LIMIT = PIf / 2.3f;
	const float MOUSE_ROT_SPEED = 0.003f;

	const Vec3 FRONT_VEC_TEMP = { 0,0,1.0f };
	Vec3 frontVec_ = FRONT_VEC_TEMP;

	Vec2 oldMousePos_ = { 0,0 };

	Object3D* parent_ = nullptr;

	Vec2 mMouseSensitivity;

	CameraTab* mCameraTab;

private:
	void Jump();
	void MoveUpdate();
	void JumpUpdate();

	void DirectionUpdate();

	Vec3 MinLengthVec3(const Vec3& vec, float maxLength);

public:
	void Init();
	void Update();
	void Draw();
	void CopyComponent(IComponent* src);

	DefDel;

public:
	// マウスの感度取得や設定
	Vec2 GetMouseSensitivity();
	void SetMouseSensitivity(Vec2 mouseSensitivity);

	Vec3 GetMoveVec() {
		return moveVec_
			;
	}

	const Vec3& GetFrontVec() { return frontVec_; }

	void GravityToZero();
	Gravity* GetGravity();
};

#ifndef PlayerControl_RegisterDef
#define PlayerControl_RegisterDef
RegisterScript(PlayerControl);
#endif
