#include "IScene.h"

void IScene::UpdateAllComponents()
{
	for (auto& component : components_)
	{
		component->Update();
	}
}

void IScene::DrawAllComponents()
{
	for (auto& component : components_)
	{
		component->Draw();
	}
}
