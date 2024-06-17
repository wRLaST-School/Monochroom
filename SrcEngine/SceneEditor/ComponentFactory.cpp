#include "stdafx.h"
#include "ComponentFactory.h"

IComponent* ComponentFactory::AddChildComponent(IComponent* parent, std::string key, std::string newComponentTypeStr)
{
	return parent->AddComponent(key, GetInstance()->factory.find(newComponentTypeStr)->second());
}

void ComponentFactory::CopyComponent(IComponent* src, IComponent* parent)
{
	// まず同じ階層に同じコンポネントを追加しとく
	std::string classString = src->GetClassString();
	IComponent* dest = AddChildComponent(parent, classString, classString);

	// 自分自身をコピー
	dest->CopyComponent(src);

	// このコンポネントをコピー
	const auto& allComponents = src->GetAllComponents();
	for (const auto& comp : allComponents)
	{
		CopyComponent(comp.second.get(), dest);

		//classString = comp.second->GetClassString();
		//AddChildComponent(dest, classString, classString);
	}
}

void ComponentFactory::Register(std::string type, std::function<eastl::unique_ptr<IComponent>()> createFunc)
{
	GetInstance()->factory.emplace(type, createFunc);
}

ComponentFactory* ComponentFactory::GetInstance()
{
	static ComponentFactory ins;
	return &ins;
}
