#pragma once
#include "IScriptObject.h"
#include <Object3D.h>


class PlayerControl :
	public IScriptObject
{
private:
	const float GRAVITY = 0.003f;
	const Vec3 JUMP_POWER = { 0,0.43f,0 };
	float gravityAccel_ = 0.0f;

	bool isJump_ = false;
	Vec3 moveVec_ = { 0,0,0 };
	const float MOVE_SPEED = 0.03f;
	const float MOVE_SPEED_MAX = 1.2f;
	const float MOVE_ATTEN = 0.87f;

	Object3D* parent_ = nullptr;

private:
	void Jump();
	void MoveUpdate();
	void JumpUpdate();

	Vec3 MinLengthVec3(const Vec3& vec, float maxLength);

public:
	void Init();
	void Update();
	void Draw();
	void CopyComponent(IComponent* src);

	DefDel;
};

RegisterScript(PlayerControl);

