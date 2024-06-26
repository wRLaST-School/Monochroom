#include "PlayerControl.h"
#include <Input.h>
#include <ScriptComponent.h>
#include <AppOperationCommand.h>
#include <GameManager.h>
#include <ConsoleWindow.h>

using namespace Input;

void PlayerControl::Init()
{
	parent_ = This()->Parent()->CastTo<Object3D>();
	mGravity = std::make_unique<Gravity>();
	//parent_->rotationE.z = 180.0f;
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
	if (isJump_)
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
	Vec3 mVec = AppOperationCommand::GetInstance()->PlayerMoveCommand() * MOVE_SPEED;

	//正面ベクトルと横ベクトルを使って向きに合わせた移動
	Vec3 front = frontVec_.GetNorm() * mVec.z;
	Vec3 right = Vec3({ 0,1.0f,0 }).Cross(frontVec_.GetNorm());
	right *= mVec.x;

	//規定のスピード超えないように
	mVec = MinLengthVec3(Vec3{ moveVec_.x,0,moveVec_.z } + front + right, MOVE_SPEED_MAX);
	moveVec_.x = mVec.x;
	moveVec_.z = mVec.z;

	//移動
	parent_->position += {moveVec_.x, 0, moveVec_.z};
}


//-------------------------------------------------------------------------------
void PlayerControl::CameraUpdate()
{
	Vec3 mouseMoveVec = AppOperationCommand::GetInstance()->PlayerAngleCommand();

	parent_->rotationE.y += mouseMoveVec.x * MOUSE_ROT_SPEED;
	parent_->rotationE.x += mouseMoveVec.y * MOUSE_ROT_SPEED;

	if (parent_->rotationE.y > PIf * 2.0f)
	{
		parent_->rotationE.y = 0;
	}
	else if (parent_->rotationE.y < 0)
	{
		parent_->rotationE.y = PIf * 2.0f;
	}

	parent_->rotationE.x = std::min(parent_->rotationE.x, ANGLE_LIMIT);
	parent_->rotationE.x = std::max(parent_->rotationE.x, -ANGLE_LIMIT);

	//正面回転
	frontVec_.x = -(FRONT_VEC_TEMP.x * cosf(parent_->rotationE.y) - FRONT_VEC_TEMP.z * sinf(parent_->rotationE.y));
	frontVec_.z = FRONT_VEC_TEMP.x * sinf(parent_->rotationE.y) + FRONT_VEC_TEMP.z * cosf(parent_->rotationE.y);
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
	if (GameManager::GetInstance()->GetisStop())
	{
		return;
	}
	ConsoleWindow::Log(std::format("PlayerControl::isStop : {}", GameManager::GetInstance()->GetisStop()));

	//ジャンプ
	if (AppOperationCommand::GetInstance()->PlayerJumpCommand() && !isJump_)
	{
		Jump();
	}

	moveVec_ += mGravity->CalcGravity();

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

void PlayerControl::GravityToZero()
{
	if (moveVec_.y <= 0.0f)
	{
		isJump_ = false;
		gravityAccel_ = 0;
		moveVec_.y = 0;
	}
}

RegisterScriptBody(PlayerControl);