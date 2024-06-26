#include "stdafx.h"
#include "Collision.h"
#include <Util.h>

bool Collision::RayTriggerSphere(RayCollider* ray, SphereCollider* obb2)
{
	// レイの起点から球の中心へのベクトル
	Vec3 oc = ray->r.origin - obb2->pos;

	// レイの方向ベクトルの長さの二乗
	float rayDirDot = ray->r.ray.Dot(ray->r.ray);

	// 2倍のレイの方向ベクトルとocのドット積
	float raySphereDot = 2.0f * oc.Dot(ray->r.ray);

	// ocの長さの二乗から球の半径の二乗を引いた値
	float diff = oc.Dot(oc) - obb2->r * obb2->r;

	// 判別式
	float discriminant = raySphereDot * raySphereDot - 4 * rayDirDot * diff;

	// 判別式が0以上なら交差している可能性がある
	if (discriminant < 0)
	{
		ray->color = Color::White;
		obb2->color = Color::White;
		return false;
	}

	// 判別式が0以上の場合、2つの解を求める
	float sqrtDiscriminant = sqrt(discriminant);
	float t1 = (-raySphereDot - sqrtDiscriminant) / (2.0f * rayDirDot);
	float t2 = (-raySphereDot + sqrtDiscriminant) / (2.0f * rayDirDot);

	// tがレイの長さの範囲内にあるかどうかを確認する
	if ((t1 >= 0 && t1 <= ray->r.length) ||
		(t2 >= 0 && t2 <= ray->r.length))
	{
		ray->color = Color::Red;
		obb2->color = Color::Red;
		return true;
	}

	ray->color = Color::White;
	obb2->color = Color::White;
	return false;
}

bool Collision::OBBTriggerOBB(OBBCollider* obb1, OBBCollider* obb2)
{
	if (!obb1->isActive || !obb2->isActive)
	{
		return false;
	}

	std::array<Vec3, 15> tempAxes;

	// 各OBBの軸
	Matrix aRotMat = obb1->rot.GetRotMat();
	Matrix bRotMat = obb2->rot.GetRotMat();
	tempAxes[0] = aRotMat.ExtractAxisX().GetNorm();
	tempAxes[1] = aRotMat.ExtractAxisY().GetNorm();
	tempAxes[2] = aRotMat.ExtractAxisZ().GetNorm();
	tempAxes[3] = bRotMat.ExtractAxisX().GetNorm();
	tempAxes[4] = bRotMat.ExtractAxisY().GetNorm();
	tempAxes[5] = bRotMat.ExtractAxisZ().GetNorm();

	// 軸のクロス積
	int index = 6;
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			tempAxes[index++] = Vec3::Cross(tempAxes[i], tempAxes[j + 3]);
		}
	}

	for (auto axis : tempAxes)
	{
		if (axis.x == 0 && axis.y == 0 && axis.z == 0)
		{
			continue;
		}

		if (!obb1->CheckSeparationAxis(obb2, axis))
		{
			obb1->color = Color::White;
			obb2->color = Color::White;
			return false;
		}
	}

	//分離できないため当たっている
	obb1->color = Color::Red;
	obb2->color = Color::Red;
	return true;
}

bool Collision::OBBTriggerSphere(OBBCollider* obb, SphereCollider* sphere, Vec3* pushOut)
{
	// OBBの中心から球の中心までのベクトル
	Vec3 diff = sphere->pos - obb->pos;

	// OBBの中心から球の中心までの最近接点を求める用
	Vec3 closestPoint = obb->pos;

	// 軸ベクトルを取得
	Matrix rotMat = obb->rot.GetRotMat();

	Vec3 xAxis = rotMat.ExtractAxisX().GetNorm();
	Vec3 yAxis = rotMat.ExtractAxisY().GetNorm();
	Vec3 zAxis = rotMat.ExtractAxisZ().GetNorm();

	Vec3 distanceAlongAxes = Vec3(
		Vec3::Dot(diff, xAxis),
		Vec3::Dot(diff, yAxis),
		Vec3::Dot(diff, zAxis));
	distanceAlongAxes.x = Util::Clamp<float>(distanceAlongAxes.x, -obb->scale.x, obb->scale.x);
	distanceAlongAxes.y = Util::Clamp<float>(distanceAlongAxes.y, -obb->scale.y, obb->scale.y);
	distanceAlongAxes.z = Util::Clamp<float>(distanceAlongAxes.z, -obb->scale.z, obb->scale.z);

	//distanceAlongAxes.Clamp(-obb->scale, obb->scale);

	closestPoint += xAxis * distanceAlongAxes.x;
	closestPoint += yAxis * distanceAlongAxes.y;
	closestPoint += zAxis * distanceAlongAxes.z;

	// 最近接点と球の中心までの距離を計算
	float squaredDis = (closestPoint - sphere->pos).GetSquaredLength();
	float r2 = sphere->r * sphere->r;

	// 球の半径の二乗と比較して判定
	if (squaredDis <= r2)
	{
		diff = (closestPoint - sphere->pos);
		obb->color = Color::Red;
		sphere->color = Color::Red;

		if (pushOut)
		{
			*pushOut = -diff.GetNorm() * (sphere->r - diff.GetLength());
		}

		return true;
	}

	obb->color = Color::White;
	sphere->color = Color::White;
	return false;
}

bool Collision::OBBTriggerRay(OBBCollider* obb, RayCollider* ray)
{
	obb->color = Color::White;
	ray->color = Color::White;

	float tMin = 0.f;
	float tMax = ray->r.length;

	std::array<Vec3, 3> axes;
	Matrix aRotMat = obb->rot.GetRotMat();
	axes[0] = aRotMat.ExtractAxisX().GetNorm();
	axes[1] = aRotMat.ExtractAxisY().GetNorm();
	axes[2] = aRotMat.ExtractAxisZ().GetNorm();

	Vec3 p = obb->pos - ray->r.origin;
	for (int i = 0; i < 3; ++i)
	{
		float e = axes[i].Dot(p);
		float f = axes[i].Dot(ray->r.ray);

		// レイが平面に平行でない場合
		if (std::fabs(f) > 0.001f)
		{
			float t1 = (e + obb->scale.x * (i == 0) + obb->scale.y * (i == 1) + obb->scale.z * (i == 2)) / f;
			float t2 = (e - obb->scale.x * (i == 0) - obb->scale.y * (i == 1) - obb->scale.z * (i == 2)) / f;

			if (t1 > t2) std::swap(t1, t2);

			tMin = std::max(tMin, t1);
			tMax = std::min(tMax, t2);

			if (tMin > tMax) return false;
		}
		else if (
			-e - obb->scale.x * (i == 0) - obb->scale.y * (i == 1) - obb->scale.z * (i == 2) > 0.0f ||
			-e + obb->scale.x * (i == 0) + obb->scale.y * (i == 1) + obb->scale.z * (i == 2) < 0.0f)
		{
			return false;
		}
	}

	obb->color = Color::Red;
	ray->color = Color::Red;
	return true;
}

