#include "PlayerControl.h"
#include <Input.h>
#include <ScriptComponent.h>

using namespace Input;



void PlayerControl::Init()
{
	parent_ = This()->Parent()->CastTo<Object3D>();
}

//-------------------------------------------
void PlayerControl::Jump()
{
	moveVec_.y = 0;
	isJump_ = true;
	moveVec_ += JUMP_POWER;
	gravityAccel_ = 0;
}

void PlayerControl::JumpUpdate()
{
	//とりあえず仮で
	if (parent_->position.y < 0.0f)
	{
		parent_->position.y = 0;
		isJump_ = false;
		gravityAccel_ = 0;
		moveVec_.y = 0;
	}
	else if (isJump_)
	{
		gravityAccel_ += GRAVITY;

		moveVec_ -= {0, gravityAccel_, 0};

		parent_->position += {0, moveVec_.y, 0};
	}
}

void PlayerControl::MoveUpdate()
{
	//移動ベクトル減衰
	moveVec_.x *= MOVE_ATTEN;
	moveVec_.z *= MOVE_ATTEN;

	//移動
	Vec3 mVec = MinLengthVec3({
		(
			(Key::Down(DIK_RIGHTARROW) || Key::Down(DIK_D))
		- (Key::Down(DIK_LEFTARROW) || Key::Down(DIK_A))
			) * MOVE_SPEED,
			0,
			(
				(Key::Down(DIK_UPARROW) || Key::Down(DIK_W))
		- (Key::Down(DIK_DOWNARROW) || Key::Down(DIK_S))
				) * MOVE_SPEED
		},
		MOVE_SPEED);


	//規定のスピード超えないように
	mVec = MinLengthVec3(Vec3{ moveVec_.x,0,moveVec_.z } + mVec, MOVE_SPEED_MAX);
	moveVec_.x = mVec.x;
	moveVec_.z = mVec.z;


	parent_->position += {moveVec_.x, 0, moveVec_.z};
}


//-------------------------------------------------------------------------------
void PlayerControl::CameraUpdate()
{
	Vec2 mousePos = Mouse::GetPos();

	Vec2 mouseMove = mousePos - oldMousePos_;

	parent_->rotationE.y += mouseMove.x * MOUSE_ROT_SPEED;
	parent_->rotationE.x += mouseMove.y * MOUSE_ROT_SPEED;

	parent_->rotationE.x = std::min(parent_->rotationE.x, ANGLE_LIMIT);
	parent_->rotationE.x = std::max(parent_->rotationE.x, -ANGLE_LIMIT);

	Quaternion q = Quaternion::EulerToQuaternion(parent_->rotationE);

	//Quaternion::

	oldMousePos_ = mousePos;
}


//--------------------------------------------------------------------------------
Vec3 PlayerControl::MinLengthVec3(const Vec3& vec, float maxLength)
{
	Vec3 ansVec = vec;

	if (ansVec.GetLength() > maxLength)
	{
		ansVec = ansVec.GetNorm() * maxLength;
	}

	return Vec3(ansVec);
}

//-------------------------------------------
void PlayerControl::Update()
{
	//ジャンプ
	if (Key::Triggered(DIK_SPACE) && !isJump_)
	{
		Jump();
	}

	//ジャンプ更新
	JumpUpdate();

	//
	CameraUpdate();

	//移動更新
	MoveUpdate();
}

void PlayerControl::Draw()
{

}

void PlayerControl::CopyComponent(IComponent* src)
{

}
