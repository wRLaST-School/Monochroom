#pragma once
#include <Essentials.h>

class DLLExport Line
{
public:
	Line(const Float3& start, const Float3& end, const Float4& color) : start(start), end(end), color(color) {}
	Float3 start;
	Float3 end;

	Float4 color;
};

class LineDrawer
{
public:
	static void Init();

	static DLLExport void DrawLine(const Float3& start, const Float3& end, const Float4& color);
	static DLLExport void DrawLines(const std::vector<Float3>& positions, const Float4& color);
	static DLLExport void DrawSphere(const Float3& center, const float radius, const Float4& color, const int segments = 16, const int ring = 8);
	static DLLExport void DrawCube(const Float3& center, const Float3& scale, const Float4& color);
	static DLLExport void DrawRotaCube(const Float3& center, const Float3& scale, const Float3& rot, const Float4& color);
	static DLLExport void DrawRotaCube(const Float3& center, const Float3& scale, const Quaternion& rot, const Float4& color);

	static void DrawAllLines();

	static void ClearLineData();
private:
	static std::vector<Line> lines_;

	static const int32_t MAX_LINES = 4096;

	static D3D12_VERTEX_BUFFER_VIEW vbView_;
	static Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff_;
	static Line* vertMap_;
};

namespace LineDrawerCommon {
	// 頂点レイアウト
	static D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
			{
				"FROM", 0,
				DXGI_FORMAT_R32G32B32_FLOAT, 0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
			},
			{
				"TO", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
			},
			{
				"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
			}
	};
}

