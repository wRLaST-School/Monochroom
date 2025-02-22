#include "stdafx.h"
#include "Quaternion.h"
#include <Util.h>

Quaternion::Quaternion()
{
	w = 1.f;
	v = Vec3(0.f, 0.f, 0.f);
}

Quaternion::Quaternion(float w, float x, float y, float z) : w(w)
{
	v = Vec3(x, y, z);
}

Quaternion::Quaternion(Vec3 axis, float rad)
{
	w = cosf(rad / 2.f);
	v = axis.SetLength(sinf(rad / 2.f));
}

Quaternion::Quaternion(float real, Vec3 imaginary)
{
	w = real;
	v = imaginary;
}

Quaternion::Quaternion(Vec3 v)
{
	this->v = v;
	this->w = 0.f;
}

Quaternion Quaternion::operator*(const Quaternion& o) const
{
	return Quaternion(w * o.w - v.Dot(o.v),
		v.Cross(o.v) + (v * o.w) + (o.v * w)
	);
}

Quaternion Quaternion::operator*(const float& o) const
{
	return Quaternion(w * o, v * o);
}

Quaternion& Quaternion::operator*=(const Quaternion& o)
{
	Quaternion t = *this * o;
	*this = t;

	return *this;
}

bool Quaternion::operator==(const Quaternion& o) const
{
	return v.x == o.v.x && v.y == o.v.y && v.z == o.v.z && w == w;
}

Quaternion Quaternion::Identity()
{
	return Quaternion();
}

Quaternion Quaternion::Conjugate(const Quaternion& q)
{
	return Quaternion(q.w, -q.v);
}

Quaternion Quaternion::GetNormalized(const Quaternion& q)
{
	Quaternion temp = q;
	temp.Normalize();
	return temp;
}

Quaternion Quaternion::GetInverse(const Quaternion& q)
{
	float norm = q.GetNorm();
	norm *= norm;

	Quaternion conj = Conjugate(q);

	return Quaternion(conj.w / norm, Vec3(conj.v.x / norm, conj.v.y / norm, conj.v.z / norm));
}

Quaternion Quaternion::Slerp(const Quaternion& zero, const Quaternion& one, const float& t)
{
	float clamped = Util::Clamp(zero.Dot(one), -1.f, 1.f);
	float theta = acosf(clamped);

	if (zero == one)
	{
		return zero;
	}

	if (!theta)
	{
		return zero;
	}

	Quaternion o = one;
	if (clamped < 0)
	{
		o.v = Vec3(-one.v.x, -one.v.y, -one.v.z);
		o.w = -one.w;
	}

	Quaternion zerot = o * (sinf((1.f - t) * theta) / sinf(theta));
	Quaternion onet = (one * (sinf(t * theta) / sinf(theta)));
	Quaternion result;

	result.w = zerot.w + onet.w;
	result.v = zerot.v + onet.v;

	result.Normalize();
	return result;
}

Quaternion Quaternion::DirToDir(const Vec3& from, const Vec3& to)
{
	Vec3 f = from;
	Vec3 t = to;

	f.Norm();
	t.Norm();

	float dot = f.Dot(t);

	Vec3 cross = f.Cross(t);

	if (cross.GetSquaredLength() == 0)
	{
		Vec3 axis(0.f, 1.f, 0.f);

		float theta = 0.f;

		return Quaternion(axis, theta);
	}

	Vec3 axis = cross.Norm();

	dot = Util::Clamp(dot, -1.f, 1.f);
	float theta = acosf(dot);

	return Quaternion(axis, theta);
}

Quaternion Quaternion::DirToDir(const Vec3& from, const Vec3& to, const float maxRad)
{
	Vec3 f = from;
	Vec3 t = to;

	f.Norm();
	t.Norm();

	float dot = f.Dot(t);

	Vec3 cross = f.Cross(t);

	if (cross.GetSquaredLength() == 0 || (f - t).GetSquaredLength() == 0)
	{
		Vec3 axis(0.f, 1.f, 0.f);

		float theta = 0.f;

		return Quaternion(axis, theta);
	}

	Vec3 axis = cross.Norm();

	dot = Util::Clamp(dot, -1.f, 1.f);

	float theta = acosf(dot);

	theta = Util::Clamp(theta, 0.f, maxRad);

	return Quaternion(axis, theta);
}

Quaternion Quaternion::EulerToQuaternion(const Float3& xyz)
{
	float rollCos = cosf(xyz.z / 2.f);
	float pitchCos = cosf(xyz.x / 2.f);
	float yawCos = cosf(xyz.y / 2.f);

	float rollSin = sinf(xyz.z / 2.f);
	float pitchSin = sinf(xyz.x / 2.f);
	float yawSin = sinf(xyz.y / 2.f);

	return Quaternion(
		rollCos * pitchCos * yawCos + rollSin * pitchSin * yawSin,
		Vec3(
			rollCos * pitchSin * yawCos + rollSin * pitchCos * yawSin,
			rollCos * pitchCos * yawSin - rollSin * pitchSin * yawCos,
			rollSin * pitchCos * yawCos - rollCos * pitchSin * yawSin
		)
	);
}

Quaternion Quaternion::AngleAxis(const Vec3 axis, float angle)
{
	Vec3 normAxis = axis.GetNorm();
	float halfAngle = ConvertAngleToRadian(angle) / 2.0f;
	float s = sinf(halfAngle);

	return Quaternion(
		cosf(halfAngle),
		normAxis.x * s,
		normAxis.y * s,
		normAxis.z * s
	);
}

Vec3 Quaternion::AnyAxisRotation(const Vec3 vec, const Vec3 axis, const float radian)
{
	Quaternion result = Quaternion(0, vec);

	Vec3 normAxis = axis.GetNorm();

	// 軸を正規化
	Quaternion q = Quaternion(cosf(radian / 2), normAxis * sinf(radian / 2));

	// 軸回転がクォータニオンの逆クォータニオンを作成
	Quaternion qc = Quaternion::Conjugate(q);

	// 軸回転を適用
	result = q * result;
	result = result * qc;

	return result.v;
}

float Quaternion::GetNorm() const
{
	return sqrtf(w * w + v.x * v.x + v.y * v.y + v.z * v.z);
}

Quaternion& Quaternion::Normalize()
{
	float norm = GetNorm();
	w /= norm;
	v.x /= norm;
	v.y /= norm;
	v.z /= norm;

	return *this;
}

Quaternion& Quaternion::Inverse()
{
	*this = GetInverse(*this);

	return *this;
}

Matrix Quaternion::GetRotMat() const
{
	Vec3 axis = v;
	Float4 X = { axis.x * axis.x, axis.x * axis.y, axis.x * axis.z, axis.x * w };
	Float3 Y = { axis.y * w, axis.y * axis.y, axis.y * axis.z };
	Float2 Z = { axis.z * axis.z, axis.z * w };
	return Matrix(
		1 - 2 * Y.y - 2 * Z.x,
		2 * X.y + 2 * Z.y,
		2 * X.z - 2 * Y.x,
		0,

		2 * X.y - 2 * Z.y,
		1 - 2 * X.x - 2 * Z.x,
		2 * Y.z + 2 * X.w,
		0,

		2 * X.z + 2 * Y.x,
		2 * Y.z - 2 * X.w,
		1 - 2 * X.x - 2 * Y.y,
		0,

		0, 0, 0, 1
	);
}

float Quaternion::Dot(const Quaternion& o) const
{
	return v.x * o.v.x + v.y * o.v.y + v.z * o.v.z + w * o.w;
}

Vec3 operator*(Vec3 v, Quaternion q)
{
	if (q.w == 0) return v;
	return (q * Quaternion(v) * Quaternion::Conjugate(q)).v;
}
