#pragma once
/*****************************************************************//**
 * @file   HierarchyPanel.h
 * @brief  エディタ画面のHierarchyPanelの表示と処理に関するファイル
 * 
 * @author Wrelf
 *********************************************************************/

#include <DLLDef.h>

/**
 * @brief HierarchyPanel用のクラス
 */
class HierarchyPanel final
{
public:
	/**
	 * @brief 描画処理
	 * 
	 */
	void Draw();

	/**
	 * @brief ImGui描画時の処理
	 * 
	 */
	void OnImGuiRender();

	/**
	 * @brief 再帰的にリソースを表示する関数
	 * 
	 * @param current 現在の対象コンポーネント
	 */
	void ShowItemRecursive(IComponent* current);

	/**
	 * @brief ドラッグドロップ受付処理
	 * 
	 * @param current 対象コンポーネント
	 */
	void DragDropTarget(IComponent* current);

	/**
	 * @brief ドラッグドロップ受付結果が画像だった場合
	 * 
	 * @param current 対象コンポーネント
	 */
	void DDTargetTexture(IComponent* current);

	/**
	 * @brief ドラッグドロップ受付結果がモデルだった場合
	 * 
	 * @param current 対象コンポーネント
	 */
	void DDTargetModel(IComponent* current);

	/**
	 * @brief ドラッグドロップ受付結果が他のコンポーネントで親子関係を変更する場合
	 *
	 * @param current 対象コンポーネント
	 */
	void DDTargetReParent(IComponent* current);
	/**
	 * @brief ドラッグドロップ受付結果が他のコンポーネントかつ移動前にSHIFTが押されていて順番を変更する場合
	 *
	 * @param current 対象コンポーネント
	 */
	void DDTargetReOrder(IComponent* current);
	/**
	 * @brief Static描画関数
	 * 
	 */
	static void SDraw();

	// ポップ画面
	void PopWindow();

private:

	eastl::map<std::string, int32_t> itemIndexByName;
	IComponent* selected;
	IComponent* copy;
	bool reorder = false;
public://singleton
	HierarchyPanel(const HierarchyPanel&) = delete;
	HierarchyPanel(HierarchyPanel&&) = delete;
	HierarchyPanel& operator=(const HierarchyPanel&) = delete;
	HierarchyPanel& operator=(HierarchyPanel&&) = delete;

	static HierarchyPanel* GetInstance();

private:
	HierarchyPanel() = default;
	~HierarchyPanel() = default;
};

