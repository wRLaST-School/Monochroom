#include "stdafx.h"
#include "IComponent.h"
#include "ComponentFactory.h"

#pragma warning (push)
#pragma warning (disable:26800)
#include <SrcExternal/json.hpp>
#include <InspectorWindow.h>
#pragma warning (pop)

#include <SpImGui.h>
#include <ScriptComponent.h>

using namespace nlohmann;


IComponent* IComponent::AddComponent(const std::string& key, eastl::unique_ptr<IComponent> component)
{
	components_.emplace_back(eastl::move(component));
	auto& back = components_.back();
	back->name_ = key;
	back->parent_ = this;

	return back.get();
}

//void IComponent::CopyComponent(IComponent* src)
//{
//	name_ = src->name_;
//
//	for (auto& comp : src->components_)
//	{
//		// コンポネントを追加
//		std::string classString = comp.second->GetClassString();
//		std::string key = ComponentFactory::GetInstance()->factory.find(classString)->first;
//		ComponentFactory::AddChildComponent(this, key, key);
//	}
//
//	active = src->active;
//	parent_ = src->parent_;
//}

void IComponent::ChangeParent(IComponent* newParent)
{
	for (auto itr = parent_->components_.begin(); itr != parent_->components_.end(); itr++)
	{
		if ((*itr).get() == this)
		{
			newParent->components_.emplace_back(std::move((*itr)));
			parent_->components_.erase(itr);
			parent_ = newParent;
			return;
		}
	}
}

void IComponent::RemoveComponent(const std::string& key)
{
	for (auto itr = components_.begin(); itr != components_.end(); itr++)
	{
		if ((*itr)->name_ == key)
		{
			childRemovedNewItr_ = components_.erase(itr);
			return;
		}
	}
}

void IComponent::RemoveComponent(IComponent* ptr)
{
	for (auto itr = components_.begin(); itr != components_.end(); itr++)
	{
		if (itr->get() == ptr)
		{
			childRemovedNewItr_ = components_.erase(itr);
			return;
		}
	}
}
//
//void IComponent::ClearComponentWithKey(const std::string& key)
//{
//	for (auto itr = components_.begin(); itr != components_.end(); itr++)
//	{
//		if ((*itr)->name_ == key)
//		{
//			childRemovedNewItr_ = components_.erase(itr);
//			return;
//		}
//	}
//}

void IComponent::ClearAllComponents()
{
	components_.clear();
}

IComponent* IComponent::GetComponent(const std::string& key)
{
	for (auto& c : components_)
	{
		if (c->name_ == key)
		{
			return c.get();
		}
	}

	return nullptr;
}

eastl::list<IComponent*> IComponent::GetComponents(const std::string& key)
{
	eastl::list<IComponent*> hitComponents;

	for (auto& c : components_)
	{
		if (c->name_ == key)
		{
			hitComponents.emplace_back(c.get());
		}
	}

	return hitComponents;
}

IComponent* IComponent::Parent()
{
	return parent_;
}

const eastl::list<eastl::unique_ptr<IComponent>>& IComponent::GetAllComponents()
{
	return components_;
}

const std::string& IComponent::GetName()
{
	return name_;
}

DLLExport bool IComponent::FindTag(const std::string& tag)
{
	for (auto& t : tags)
	{
		if (t == tag)
			return true;
	}

	return false;
}

std::string IComponent::GetSameTag(const IComponent& component, const std::string& str)
{
	for (auto& t : tags)
	{
		if (t.find(str) == std::string::npos)
		{
			continue;
		}

		for (auto& cTag : component.tags)
		{
			if (cTag.find(str) == std::string::npos)
			{
				continue;
			}

			if (cTag == t)
			{
				return t;
			}
		}
	}

	return "";
}

std::string IComponent::FindStringTag(const std::string& tag)
{
	std::string ansS = "";

	for (auto& t : tags)
	{
		size_t sPos = t.find(tag);
		if (sPos != std::string::npos)
		{
			ansS = t;
			break;
		}
	}

	return ansS;
}

DLLExport void IComponent::AddTag(const std::string& tag)
{
	tags.push_back(tag);
}

std::string IComponent::GetTag(const uint32_t index)
{
	std::string result = std::string();
	if (index < 0 || index >= tags.size())
	{
		return result;
	}

	uint32_t i = 0;
	for (auto& t : tags)
	{
		if (i == index)
		{
			result = t;
			break;
		}
		i++;
	}

	return result;
}

void IComponent::Awake()
{
}

void IComponent::Init()
{
}

void IComponent::Update()
{
}

void IComponent::LateUpdate()
{
}

void IComponent::Draw()
{
}

void IComponent::PrepDelete()
{
	deleting = true;
}

bool IComponent::CheckDelete()
{
	if (deleting)
	{
		IComponent* par = parent_;
		parent_->RemoveComponent(this);
		InspectorWindow::SelectObject(par);

		return true;
	}

	for (auto& c : components_)
	{
		if (c->CheckDelete()) {
			return true;
		};
	}

	return false;
}

DLLExport bool IComponent::IsActive()
{
	return active;
}

DLLExport void IComponent::Activate()
{
	active = true;
}

DLLExport void IComponent::Deactivate()
{
	active = false;
}

DLLExport void IComponent::ToggleActive()
{
	active = !active;
}

bool IComponent::IsScript()
{
	return dynamic_cast<ScriptComponent*>(this) ? true : false;
}

void IComponent::AwakeAllChildComponents(IComponent* parent)
{
	parent->Awake();

	if (parent->components_.size())
	{
		for (auto& c : parent->components_)
		{
			AwakeAllChildComponents(c.get());
		}
	}
}

void IComponent::InitAllChildComponents(IComponent* parent)
{
	parent->Init();

	if (parent->components_.size())
	{
		for (auto& c : parent->components_)
		{
			InitAllChildComponents(c.get());
		}
	}
}

void IComponent::UpdateAllChildComponents(IComponent* parent)
{
	parent->childRemovedNewItr_.reset();

	if (!parent->active) { return; }

	if (parent->components_.size())
	{
		for (auto itr = parent->components_.begin(); itr != parent->components_.end();)
		{
			UpdateAllChildComponents(itr->get());
			if (parent->childRemovedNewItr_)
			{
				itr = parent->childRemovedNewItr_.value();
				parent->childRemovedNewItr_.reset();
			}
			else
			{
				itr++;
			}
		}
	}

	parent->Update();
}

void IComponent::LateUpdateAllChildComponents(IComponent* parent)
{
	parent->childRemovedNewItr_.reset();

	if (!parent->active) { return; }

	if (parent->components_.size())
	{
		for (auto itr = parent->components_.begin(); itr != parent->components_.end();)
		{
			LateUpdateAllChildComponents(itr->get());
			if (parent->childRemovedNewItr_)
			{
				itr = parent->childRemovedNewItr_.value();
				parent->childRemovedNewItr_.reset();
			}
			else
			{
				itr++;
			}
		}
	}

	parent->LateUpdate();
}

void IComponent::DrawAllChildComponents(IComponent* parent)
{
	if (!parent->active) { return; }

	parent->Draw();

	if (parent->components_.size())
	{
		for (auto& c : parent->components_)
		{
			DrawAllChildComponents(c.get());
		}
	}
}

void IComponent::WriteCommonParamJson(nlohmann::json& jsonObj)
{
	nlohmann::json& arr = jsonObj["Tags"];

	for (auto& t : tags)
	{
		arr.push_back(t);
	}

	jsonObj["Name"] = name_;
	jsonObj["Active"] = active;
}

void IComponent::ReadCommonParamJson(const nlohmann::json& paramsObject)
{
	if (!paramsObject.At("Tags").is_null())
	{
		const nlohmann::json& arr = paramsObject["Tags"];

		for (auto& t : arr)
		{
			tags.push_back(t);
		}
	}

	if (!paramsObject.At("Name").is_null())
	{
		name_ = paramsObject["Name"];
	}
	if (paramsObject.contains("Active"))
	{
		active = paramsObject["Active"];
	}
}

void IComponent::OnInspectorWindowDraw()
{
}

void IComponent::CommonInspectorWindowDraw()
{
	ImGui::Checkbox("##", &active); ImGui::SameLine();
	SpImGui::InputText("Name", name_);
	//SpImGui::InputText("Name", &name_, ImGuiInputTextFlags_::ImGuiInputTextFlags_None);
}

IComponent* IComponent::GetScriptBody()
{
	auto script = dynamic_cast<ScriptComponent*>(this);
	if (script) return script->GetObj()->GetComponent();
	return nullptr;
}