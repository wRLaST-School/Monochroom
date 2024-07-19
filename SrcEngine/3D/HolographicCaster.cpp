#include "HolographicCaster.h"
#include <SpRenderer.h>
#include <Camera.h>
#include <SrHolographicCasterStage.h>

HolographicCaster::HolographicCaster()
{
	worldMat = Matrix::Identity();
}

void HolographicCaster::Update()
{
}

void HolographicCaster::Draw(Model* aModel, const TextureKey& aTexKey)
{
	if (!aModel)
	{
		return;
	}
	this->model = aModel;
	this->texKey = aTexKey == "" ? "notexture" : aTexKey;

	cbData.contents->wMat = worldMat;

	cbData.contents->vpMat =
		Camera::sCurrent->GetViewMat() *
		Camera::sCurrent->GetProjMat();

	cbData.contents->cameraPos = Camera::sCurrent->position;

	SpRenderer::DrawCommand([=]
		{
			GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(0, cbData.buffer->GetGPUVirtualAddress());
			GetSpDX()->cmdList->SetGraphicsRootDescriptorTable(1, SpTextureManager::GetGPUDescHandle(this->texKey));

			GetSpDX()->cmdList->IASetVertexBuffers(0, 1, &this->model->vbView);

			GetSpDX()->cmdList->IASetIndexBuffer(&this->model->ibView);

			GetSpDX()->cmdList->DrawIndexedInstanced(this->model->ibView.SizeInBytes / sizeof(uint32_t), 1, 0, 0, 0);
		}, SpRenderer::Stage::HolographicCaster);
}
