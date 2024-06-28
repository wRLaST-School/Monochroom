#pragma once
#include "SpConstBuffer.h"
#include "Model.h"
#include <Quaternion.h>

struct ConstBufferData
{
	Matrix wvpMat; // 3D変換行列
};

class ShadowCaster
{
private:
	enum class RotMode
	{
		Euler,
		Quaternion
	};

private:
	SpConstBuffer<ConstBufferData> cbData;

	Matrix worldMat;
	Model* model;

public:
	DLLExport ShadowCaster();
	void Update();
	void Draw();
};

