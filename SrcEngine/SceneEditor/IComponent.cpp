#include "stdafx.h"
#include "IComponent.h"
#include "ComponentFactory.h"

#pragma warning (push)
#pragma warning (disable:26800)
#include <SrcExternal/json.hpp>
#include <InspectorWindow.h>
#pragma warning (pop)

#include <SpImGui.h>

using namespace nlohmann;

IComponent* IComponent::AddComponent(const std::string& key, eastl::unique_ptr<IComponent> component)
{
	auto itr = components_.insert(eastl::make_pair(key, eastl::move(component)));
	itr->second->name_ = itr->first;
	itr->second->parent_ = this;

	return itr->second.get();
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
		if ((*itr).second.get() == this)
		{
			newParent->components_.emplace(name_, std::move((itr->second)));
			parent_->components_.erase(itr);
			parent_ = newParent;
			return;
		}
	}
}

void IComponent::RemoveComponent(const std::string& key)
{
	childRemovedNewItr_ = components_.erase(components_.find(key));
}

void IComponent::RemoveComponent(IComponent* ptr)
{
	for (auto itr = components_.begin(); itr != components_.end(); itr++)
	{
		if (itr->second.get() == ptr)
		{
			childRemovedNewItr_ = components_.erase(itr);
			return;
		}
	}
}

void IComponent::ClearComponentWithKey(const std::string& key)
{
	components_.erase(key);
}

void IComponent::ClearAllComponents()
{
	components_.clear();
}

IComponent* IComponent::GetComponent(const std::string& key)
{
	return components_.find(key)->second.get();
}

eastl::list<IComponent*> IComponent::GetComponents(const std::string& key)
{
	eastl::list<IComponent*> hitComponents;

	auto count = components_.count(key);
	auto itr = components_.find(key);
	for (size_t i = 0; i < count; i++)
	{
		hitComponents.emplace_back(itr->second.get());
		itr++;
	}

	return hitComponents;
}

IComponent* IComponent::Parent()
{
	return parent_;
}

const eastl::multimap<std::string, eastl::unique_ptr<IComponent>>& IComponent::GetAllComponents()
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

DLLExport void IComponent::AddTag(const std::string& tag)
{
	tags.push_back(tag);
}

void IComponent::Init()
{
}

void IComponent::Update()
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
		if (c.second->CheckDelete()) {
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

void IComponent::InitAllChildComponents(IComponent* parent)
{
	parent->Init();

	if (parent->components_.size())
	{
		for (auto& c : parent->components_)
		{
			InitAllChildComponents(c.second.get());
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
			UpdateAllChildComponents(itr->second.get());
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

void IComponent::DrawAllChildComponents(IComponent* parent)
{
	if (!parent->active) { return; }

	parent->Draw();

	if (parent->components_.size())
	{
		for (auto& c : parent->components_)
		{
			DrawAllChildComponents(c.second.get());
		}
	}
}

void IComponent::OnInspectorWindowDraw()
{
}

void IComponent::CommonInspectorWindowDraw()
{
	ImGui::Checkbox("##", &active); ImGui::SameLine();
	SpImGui::InputText("Name", &name_, ImGuiInputTextFlags_::ImGuiInputTextFlags_None);
}




