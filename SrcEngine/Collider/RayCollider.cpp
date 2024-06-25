#include "RayCollider.h"
#include <LineDrawer.h>

RayCollider::RayCollider()
{
	r.length = std::numeric_limits<float>::max();
	type = ColliderType::Ray;
	isActive = true;
	color = Color::White;
}

bool RayCollider::IsTriggerSphere(SphereCollider* other)
{
	if (!isActive || !other->isActive)
	{
		color = Color::White;
		other->color = Color::White;
		return false;
	}

	// レイの起点から球の中心へのベクトル
	Vec3 oc = r.origin - other->pos;

	// レイの方向ベクトルの長さの二乗
	float rayDirDot = r.ray.Dot(r.ray);

	// 2倍のレイの方向ベクトルとocのドット積
	float raySphereDot = 2.0f * oc.Dot(r.ray);

	// ocの長さの二乗から球の半径の二乗を引いた値
	float diff = oc.Dot(oc) - other->r * other->r;

	// 判別式
	float discriminant = raySphereDot * raySphereDot - 4 * rayDirDot * diff;

	// 判別式が0以上なら交差している可能性がある
	if (discriminant < 0)
	{
		color = Color::White;
		other->color = Color::White;
		return false;
	}

	// 判別式が0以上の場合、2つの解を求める
	float sqrtDiscriminant = sqrt(discriminant);
	float t1 = (-raySphereDot - sqrtDiscriminant) / (2.0f * rayDirDot);
	float t2 = (-raySphereDot + sqrtDiscriminant) / (2.0f * rayDirDot);

	// tがレイの長さの範囲内にあるかどうかを確認する
	if ((t1 >= 0 && t1 <= r.length) || 
		(t2 >= 0 && t2 <= r.length)) 
	{
		color = Color::Red;
		other->color = Color::Red;
		return true;
	}

	color = Color::White;
	other->color = Color::White;
	return false;
}

void RayCollider::Setting(const Vec3 aPos, const Vec3 aDir, const float aLength)
{
	r.origin = aPos;
	r.ray = aDir;
	if (aLength > 0)
	{
		r.length = aLength;
	}
	else
	{
		r.length = std::numeric_limits<float>::max();
	}
}

void RayCollider::DrawCollider()
{
	Vec3 start = r.origin;
	Vec3 end = start + r.ray.GetNorm() * r.length;
	LineDrawer::DrawLine(start, end, color);
}

bool RayCollider::Collide(const SphereCollider& other) const
{
	float length = r.ray.GetLength();

	if (length == 0) return false;

	Vec3 vToSphere = (Vec3)other.pos - this->r.origin;

	float d = r.ray.GetNorm(length).Dot(vToSphere);//SUS

	Vec3 rayTemp = r.ray;

	rayTemp.Norm(length);
	rayTemp *= d;

	Vec3 hLine = (Vec3)other.pos - (rayTemp + this->r.origin);

	return hLine.GetSquaredLength() <= other.r && d >= 0 && length >= d;
}

bool RayCollider::Collide(const PlaneCollider& other, Vec3* intersection) const
{
	//面法線とレイ方向の内積を見て裏面からのレイならfalseを返す
	float d1 = other.norm.Dot(r.ray);
	if (d1 > -EPSILON) { return false; }

	//当たり判定を見る
	float d2 = other.norm.Dot(r.origin);

	float dist = d2 - other.distance;

	float t = dist / -d1;

	//負なら当たっていない
	if (t < 0) { return false; }

	//当たったなら当たっている位置を格納してほしいならする
	if (intersection) { *intersection = r.origin + r.ray * t; }

	return true;
}