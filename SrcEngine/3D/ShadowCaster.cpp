#include "ShadowCaster.h"
#include <SpRenderer.h>

ShadowCaster::ShadowCaster()
{
	worldMat = Matrix::Identity();
	model = nullptr;
}

void ShadowCaster::Update()
{
}

void ShadowCaster::Draw()
{
	cbData.contents->wvpMat = worldMat;

	//SpRenderer::DrawCommand([&]
	//	{
	//		GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(0, cbData.buffer->GetGPUVirtualAddress());

	//		GetSpDX()->cmdList->IASetVertexBuffers(0, 1, &model->vbView);

	//		GetSpDX()->cmdList->IASetIndexBuffer(&model->ibView);

	//		GetSpDX()->cmdList->DrawIndexedInstanced(model->ibView.SizeInBytes / sizeof(uint32_t), 1, 0, 0, 0);
	//	}, SpRenderer::Stage::ShadowCaster);

}
