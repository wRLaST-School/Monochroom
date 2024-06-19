#include "stdafx.h"
#include "TitleCamera.h"
#include <Object3D.h>
#include <ScriptComponent.h>


void TitleCamera::Init()
{
	parent_ = This()->Parent()->CastTo<Object3D>();
	templatePos_ = parent_->position;
	parent_->rotMode = Object3D::RotMode::Quaternion;
}


void TitleCamera::Update()
{
	time_ += 1.0f;

	Vec2 addLength = Vec2(sinf(time_ * wiggleSpeed_.x) * wiggleLength_.x,
		sinf(time_ * wiggleSpeed_.y) * wiggleLength_.y);

	//ターゲットへの方向
	Vec3 dirVec = targetPos_ - templatePos_;
	const Vec3 DIR_VEC = dirVec.GetNorm();

	//外積で上と横を求める
	Vec3 upVec = Vec3(0, 1, 0);
	const Vec3 RIGHT_VEC = DIR_VEC.Cross(upVec);

	upVec = RIGHT_VEC.Cross(DIR_VEC);

	//移動
	parent_->position = templatePos_ + RIGHT_VEC * addLength.x + upVec * addLength.y;

	//カメラの向き
	parent_->rotation = Quaternion::DirToDir({ 0,0,1.0f }, Vec3(targetPos_ - parent_->position).GetNorm());
}
