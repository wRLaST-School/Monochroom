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

void HierarchyPanel::Draw()
{
	SpImGui::Command(std::bind(&HierarchyPanel::OnImGuiRender, this));
}

void HierarchyPanel::OnImGuiRender()
{
	itemIndex = 0;
	if (ImGui::Begin("Hierarchy"))
	{
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
			if (Input::Key::Triggered(DIK_V))
			{
				IComponent* parent = selected->parent_;
				std::string selectedName = selected->name_;

				if (parent)
				{
					ComponentFactory::CopyComponent(selected, parent);
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
	std::string taggedName = current->name_ + std::string("##") + std::to_string(itemIndex);
	itemIndex++;

	ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_None;

	nodeFlags |= ImGuiTreeNodeFlags_OpenOnArrow;
	nodeFlags |= ImGuiTreeNodeFlags_OpenOnDoubleClick;

	if (current->components_.size() == 0)
	{
		nodeFlags |= ImGuiTreeNodeFlags_Leaf;
	}

	bool treeNodeTriggered = ImGui::TreeNodeEx(taggedName.c_str(), nodeFlags);

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
			ShowItemRecursive(child.second.get());
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
