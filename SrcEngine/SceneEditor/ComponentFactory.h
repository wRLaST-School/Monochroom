#pragma once
#include <Essentials.h>

class InspectorWindow;
class HierarchyPanel;

class ComponentFactory
{
public:
	//コンポーネント追加機能で追加可能コンポーネント一覧を表示したいため
	friend InspectorWindow;
	friend HierarchyPanel;

	static IComponent* AddChildComponent(IComponent* parent, std::string key, std::string newComponentTypeStr);
	static void CopyComponent(IComponent* src, IComponent* parent);
	static DLLExport void Register(std::string type, std::function<eastl::unique_ptr<IComponent>(void)> createFunc);

public:
	eastl::unordered_map<std::string, std::function<eastl::unique_ptr<IComponent>()>> factory;

public://singleton
	ComponentFactory(const ComponentFactory&) = delete;
	ComponentFactory(ComponentFactory&&) = delete;
	ComponentFactory& operator=(const ComponentFactory&) = delete;
	ComponentFactory& operator=(ComponentFactory&&) = delete;

	static ComponentFactory* GetInstance();

private:
	ComponentFactory() = default;
	~ComponentFactory() = default;
};

