#pragma once
#include "SpConstBuffer.h"
#include "Model.h"
#include <Quaternion.h>

namespace SilhouetteObjCommon {
	// 頂点レイアウト
	static D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		}
	};
}

class Silhouette
{
private:
	SpConstBuffer<ConstBufferData> cbData;

public:
	Matrix worldMat;
	Model* model;

public:
	DLLExport Silhouette();
	void Update();
	void Draw(Model* aModel);
};

