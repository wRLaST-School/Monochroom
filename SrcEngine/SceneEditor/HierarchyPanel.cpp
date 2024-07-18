#include "stdafx.h"
#include "HierarchyPanel.h"
#include <SpImGui.h>
#include <IComponent.h>
#include <SceneManager.h>
#include <InspectorWindow.h>
#include <Object3D.h>
#include <Input.h>
#include <SpriteObject.h>
#include <ComponentFactory.h>
#include <format>

void HierarchyPanel::Draw()
{
	SpImGui::Command(std::bind(&HierarchyPanel::OnImGuiRender, this));
}

void HierarchyPanel::OnImGuiRender()
{
	itemIndexByName.clear();
	if (ImGui::Begin("Hierarchy"))
	{
		ImGui::Checkbox("Reorder", &reorder);

		ImGui::Separator();

		ShowItemRecursive(SceneManager::currentScene.get());

		PopWindow();

		// ウィンドウ外の時return
		if (!ImGui::IsWindowHovered())
		{
			ImGui::End();
			return;
		}

		if (Input::Key::Down(DIK_LCONTROL))
		{
			if (Input::Key::Triggered(DIK_C))
			{
				copy = selected;
			}

			if (Input::Key::Triggered(DIK_V))
			{
				IComponent* parent = copy->parent_;

				if (parent)
				{
					ComponentFactory::CopyComponent(copy, parent);
				}
			}
		}

		if (Input::Key::Triggered(DIK_DELETE))
		{
			if (!(InspectorWindow::GetSelected<IScene>()))
			{
				IComponent* cmp = InspectorWindow::GetSelected<IComponent>();
				if (cmp)
				{
					cmp->PrepDelete();
				}
			}
		}
	}

	ImGui::End();
}

void HierarchyPanel::ShowItemRecursive(IComponent* current)
{
	//同じ名前ならIndexを加算
	itemIndexByName[current->name_]++;

	std::string taggedName = current->name_ + std::string("##") + std::to_string(itemIndexByName[current->name_]);

	ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_None;

	nodeFlags |= ImGuiTreeNodeFlags_OpenOnArrow;
	nodeFlags |= ImGuiTreeNodeFlags_OpenOnDoubleClick;

	if (current->components_.size() == 0)
	{
		nodeFlags |= ImGuiTreeNodeFlags_Leaf;
	}

	ImVec4 col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	if (!current->IsActive())
	{
		col = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
	}
	ImGui::PushStyleColor(ImGuiCol_Text, col);
	bool treeNodeTriggered = ImGui::TreeNodeEx(taggedName.c_str(), nodeFlags);
	ImGui::PopStyleColor(1); // 3つのスタイルカラーをポップ

	if (reorder)
	{
		if (ImGui::BeginDragDropSource())
		{
			ImGui::SetDragDropPayload("HIERARCHY_ITEM_COMPONENT_REORDER", 
				&current, 
				sizeof(IComponent**), 
				ImGuiCond_Once);
			ImGui::EndDragDropSource();
		}
	}
	else
	{
		if (ImGui::BeginDragDropSource())
		{
			ImGui::SetDragDropPayload("HIERARCHY_ITEM_COMPONENT", &current, sizeof(IComponent**), ImGuiCond_Once);
			ImGui::EndDragDropSource();
		}
	}

	DragDropTarget(current);

	if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
	{
		InspectorWindow::SelectObject(current);
		selected = current;
	}

	if (treeNodeTriggered)
	{
		for (auto& child : current->components_)
		{
			ShowItemRecursive(child.get());
		}

		ImGui::TreePop();
	}
}

void HierarchyPanel::DragDropTarget(IComponent* current)
{
	if (ImGui::BeginDragDropTarget())
	{
		DDTargetTexture(current);
		DDTargetModel(current);
		DDTargetReParent(current);
		DDTargetReOrder(current);

		ImGui::EndDragDropTarget();
	}
}

void HierarchyPanel::DDTargetTexture(IComponent* current)
{
	const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("RES_WINDOW_ITEM_TEXTURE");

	if (payload) {
		const char* texKey = reinterpret_cast<const char*>(payload->Data);

		Object3D* obj = dynamic_cast<Object3D*>(current);

		if (obj)
		{
			obj->texture = texKey;
		}

		SpriteObject* spr = dynamic_cast<SpriteObject*>(current);

		if (spr)
		{
			spr->tex = texKey;
		}
	}
}

void HierarchyPanel::DDTargetModel(IComponent* current)
{
	const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("RES_WINDOW_ITEM_MODEL");

	if (payload) {
		const char* modelKey = reinterpret_cast<const char*>(payload->Data);

		Object3D* obj = dynamic_cast<Object3D*>(current);

		if (obj)
		{
			obj->model = ModelManager::GetModel(modelKey);
		}
	}
}

void HierarchyPanel::DDTargetReParent(IComponent* current)
{
	const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HIERARCHY_ITEM_COMPONENT");

	if (payload) {
		IComponent* child = *reinterpret_cast<IComponent**>(payload->Data);
		if (child != current)
			child->ChangeParent(current);

		Object3D* castCurrent = dynamic_cast<Object3D*>(current);
		Object3D* castChild = dynamic_cast<Object3D*>(child);
		if (castCurrent && castChild)
		{
			castChild->parent = castCurrent;
		}
	}
}

void HierarchyPanel::DDTargetReOrder(IComponent* current)
{
	const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HIERARCHY_ITEM_COMPONENT_REORDER");

	if (payload) {
		IComponent* cmp = *reinterpret_cast<IComponent**>(payload->Data);
		if (cmp == nullptr) return;
		if (cmp == current) return;
		if (current->parent_ != cmp->parent_) return;

		auto moveitr = current->parent_->components_.end();
		auto curitr = current->parent_->components_.end();

		for (auto itr = current->parent_->components_.begin(); itr != current->parent_->components_.end(); itr++)
		{
			if (itr->get() == cmp) { moveitr = itr; continue; }
			if (itr->get() == current) { curitr = itr; continue; }
		}

		if (moveitr == current->parent_->components_.end()) return;
		if (curitr == current->parent_->components_.end()) return;

		eastl::unique_ptr<IComponent> temp = eastl::move(*moveitr);
		current->parent_->components_.insert(curitr, eastl::move(temp));
		current->parent_->components_.erase(moveitr);
	}
}

void HierarchyPanel::SDraw()
{
	GetInstance()->Draw();
}

void HierarchyPanel::PopWindow()
{
	if (ImGui::IsWindowHovered() &&
		ImGui::IsMouseReleased(ImGuiMouseButton_Right))
	{
		ImGui::OpenPopup("Select Component Popup");
	}

	//コンポーネント選択ポップアップの設定
	if (ImGui::BeginPopup("Select Component Popup"))
	{
		for (auto& item : ComponentFactory::GetInstance()->factory)
		{
			if (ImGui::MenuItem(item.first.c_str()))
			{
				ComponentFactory::AddChildComponent(selected, item.first.c_str(), item.first.c_str());
			}
			ImGui::Separator();
		}

		ImGui::EndPopup();
	}
}

HierarchyPanel* HierarchyPanel::GetInstance()
{
	static HierarchyPanel ins;
	return &ins;
}
