#include "ShadowCaster.h"
#include <SpRenderer.h>
#include <Camera.h>
#include <SrShadowCasterStage.h>

ShadowCaster::ShadowCaster()
{
	worldMat = Matrix::Identity();
}

void ShadowCaster::Update()
{
}

void ShadowCaster::Draw(Model* aModel)
{
	if (!aModel)
	{
		return;
	}
	this->model = aModel;

	cbData.contents->wMat = worldMat;
	//cbData.contents->vpMat =
	//	SrShadowCasterStage::lightCamera->GetViewMat() *
	//	SrShadowCasterStage::lightCamera->GetProjMat();

	cbData.contents->vpMat =
		Camera::sCurrent->GetViewMat() *
		Camera::sCurrent->GetProjMat();

	SpRenderer::DrawCommand([&]
		{
			GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(0, cbData.buffer->GetGPUVirtualAddress());

			GetSpDX()->cmdList->IASetVertexBuffers(0, 1, &this->model->vbView);

			GetSpDX()->cmdList->IASetIndexBuffer(&this->model->ibView);

			GetSpDX()->cmdList->DrawIndexedInstanced(this->model->ibView.SizeInBytes / sizeof(uint32_t), 1, 0, 0, 0);
		}, SpRenderer::Stage::ShadowCaster);
}
