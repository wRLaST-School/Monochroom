#include "stdafx.h"
#include "TitleCamera.h"
#include <Object3D.h>
#include <ScriptComponent.h>


void TitleCamera::Init()
{
	parent_ = This()->Parent()->CastTo<Object3D>();
	templatePos_ = parent_->position;
}


void TitleCamera::Update()
{
	time_ += 1.0f;

	Vec2 addLength = Vec2(sinf(time_ * wiggleSpeed_.x) * wiggleLength_.x,
		cosf(time_ * wiggleSpeed_.x) * wiggleLength_.x);

	//ターゲットへの方向
	Vec3 dirVec = targetPos_ - templatePos_;
	const Vec3 DIR_VEC = dirVec.GetNorm();

	//外積で上と横を求める
	Vec3 upVec = Vec3(0, 1, 0);
	const Vec3 RIGHT_VEC = upVec.Cross(dirVec);

	upVec = RIGHT_VEC.Cross(dirVec);

	//あいう
	parent_->position = templatePos_ + upVec * addLength.y + RIGHT_VEC * addLength.x;
}
