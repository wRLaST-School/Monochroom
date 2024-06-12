#pragma once
/*****************************************************************//**
 * @file   Vec3.h
 * @brief  3次元ベクトルのヘッダー
 * 
 * @author Wrelf
 *********************************************************************/
#include <vector>
#include <DLLDef.h>

struct Float3;
class Vec2;

/**
 * @briel 3次元ベクトルのクラス
 */
class DLLExport Vec3
{
public:
	// 静的メンバ
	const static Vec3 left;    // Vec3(-1,0,0) または単位ベクトル
	const static Vec3 right;   // Vec3(1,0,0) または単位ベクトル
	const static Vec3 up;      // Vec3(0,1,0) または単位ベクトル
	const static Vec3 down;    // Vec3(0,-1,0) または単位ベクトル
	const static Vec3 front;   // Vec3(0,0,1) または単位ベクトル
	const static Vec3 back;    // Vec3(0,0,-1) または単位ベクトル
	const static Vec3 one;     // Vec3(1,1,1) または単位ベクトル
	const static Vec3 zero;    // Vec3(0,0,0) または単位ベクトル

	float x;
	float y;
	float z;

	Vec3();
	Vec3(float x, float y, float z);

	//Sqrtが呼ばれる
	Vec3& SetLength(float l);

	Vec3 operator+(const Vec3& v2) const;

	Vec3 operator-(const Vec3& v2) const;
	Vec3 operator-() const;

	Vec3 operator*(float mul) const;

	Vec3 operator/(float div) const;

	Vec3& operator +=(const Vec3& v2);

	Vec3& operator -=(const Vec3& v2);

	Vec3& operator *=(float mul);

	Vec3& operator /=(float div);

	bool operator > (const Vec3& v2) const;
	bool operator >= (const Vec3& v2) const;
	bool operator < (const Vec3& v2) const;
	bool operator <= (const Vec3& v2) const;

	//Sqrtが呼ばれる
	Vec3& Norm();
	//Sqrtが呼ばれない。長さがわかっている場合に。
	Vec3& Norm(float length);

	//Sqrtが呼ばれる
	Vec3 GetNorm() const;
	//Sqrtが呼ばれない。長さがわかっている場合に。
	Vec3 GetNorm(float length) const;


	float GetLength() const;

	float GetSquaredLength() const;

	float Dot(const Vec3& v2) const;
	Vec3 Cross(const Vec3& v2) const;

	static Vec3 Lerp(const Vec3& start, const Vec3& end, const float t);
	static Vec3 Spline(const std::vector<Vec3>& points, float t);
	static float Distance(const Vec3 v1, const Vec3 v2);   // 二つのベクトル間の距離

	operator Float3() const;
	operator Vec2() const;
};

