#pragma once
#include "SpConstBuffer.h"
#include "Model.h"
#include <Quaternion.h>

namespace ShadowCasterCommon {
	// 頂点レイアウト
	static D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		}
	};
}

class ShadowCaster
{
private:
	SpConstBuffer<ConstBufferData> cbData;

public:
	Matrix worldMat;
	Model* model;

public:
	DLLExport ShadowCaster();
	void Update();
	void Draw(Model* aModel);
};

