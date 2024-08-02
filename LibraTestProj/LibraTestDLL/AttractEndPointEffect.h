#pragma once
#include<Object3D.h>

//引き寄せるブロックの終点のエフェクト
class AttractEndPointEffect
{
private:
	Vec3 mPosition;
	float mScale = 1.0f;

	const Vec3 kLeftUpFront = { -1.0f,1.0f,1.0f };
	const Vec3 kLeftUpBack = { -1.0f,1.0f,-1.0f };
	const Vec3 kLeftDownFront = { -1.0f,-1.0f,1.0f };
	const Vec3 kLeftDownBack = { -1.0f,-1.0f,-1.0f };
	const Vec3 kRightUpFront = { 1.0f,1.0f,1.0f };
	const Vec3 kRightUpBack = { 1.0f,1.0f,-1.0f };
	const Vec3 kRightDownFront = { 1.0f,-1.0f,1.0f };
	const Vec3 kRightDownBack = { 1.0f,-1.0f,-1.0f };


public:
	void Init();
	void Update();
	void Draw();

};