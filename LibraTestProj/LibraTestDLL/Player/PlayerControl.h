#pragma once
#include "IScriptObject.h"
#include <Object3D.h>
#include <Gravity.h>
#include <memory>

class PlayerControl :
	public IScriptObject
{
private:
	std::unique_ptr<Gravity> mGravity;

	const Vec3 JUMP_POWER = { 0,0.056f,0 };

	bool isJump_ = false;
	Vec3 moveVec_ = { 0,0,0 };
	const float MOVE_SPEED = 0.03f;
	const float MOVE_SPEED_MAX = 1.2f;
	const float MOVE_ATTEN = 0.87f;

	const float ANGLE_LIMIT = PIf / 2.0f;
	const float MOUSE_ROT_SPEED = 0.01f;

	const Vec3 FRONT_VEC_TEMP = { 0,0,1.0f };
	Vec3 frontVec_ = FRONT_VEC_TEMP;

	Vec2 oldMousePos_ = { 0,0 };

	Object3D* parent_ = nullptr;

	Vec2 mMouseSensitivity;

private:
	void Jump();
	void MoveUpdate();
	void JumpUpdate();

	void CameraUpdate();

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

	void GravityToZero();
	Gravity* GetGravity();
};

#ifndef PlayerControl_RegisterDef
#define PlayerControl_RegisterDef
RegisterScript(PlayerControl);
#endif
