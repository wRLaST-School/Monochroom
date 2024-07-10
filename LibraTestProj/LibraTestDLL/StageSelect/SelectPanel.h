#pragma once
#include "IScriptObject.h"
#include <Object3D.h>
#include <Vec2.h>
#include <Vec3.h>

class SelectPanel :
    public IScriptObject
{
private:
	enum PanelState
	{
		DISABLED,   // 選択されていない状態
		SELECT,     // 選択されている状態
		PRESSED,    // 押された後の状態
	};

	enum StageNum
	{
		Stage1,
		Stage2,
		Stage3,
	};
public:
	void Init();
	void Update();
	void Draw();
	void OnInspectorWindowDraw() override;
	void CopyComponent(IComponent* src);

    DefDel;
private:
	void CheckNumOver();
private:

	// ステージセレクト時に選択用のオブジェクト
	std::vector<std::vector<Object3D*>> mStageNumObj;
	std::vector<std::vector<PanelState>> mStageNumState;

	// ステージセレクト二次元配列 X Y 番号
	int mSelectMaxNumY;
	std::vector<int> mSelectMaxNumX;

	Vec2 mSelectCurrentNum;

	// ステージの状態ごとのスケール
	Vec3 mDisabledPanelScale;
	Vec3 mSelectPanelScale;
	Vec3 mPressedPanelScale;
};
RegisterScript(SelectPanel);
