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

	enum Number
	{
		ZERO,
		ONE,
		TWO,
		THREE,
		FOUR,
		FIVE,
		SIX,
		SEVEN,
		EIGHT,
		NINE,
	};

	struct StageButton
	{
		// 本体のオブジェクト
		Object3D* buttonObj;

		// ナンバーの板オブジェ
		Object3D* stageBaseNumObj;

		Object3D* hyphenObj;

		Object3D* stageSubNumObj;

		PanelState state;
	};

	enum SelectState
	{
		TITLE,
		SELECTSTAGE,
		CAMERAMOVE,
		STAGECHAGE,
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

	void InitTex();
private:

	// ステージセレクト時に選択用のオブジェクト
	std::vector<std::vector<StageButton>> mStageNum;
	//std::vector<std::vector<PanelState>> mStageNumState;

	// ステージセレクト二次元配列 X Y 番号
	int mSelectMaxNumY;
	std::vector<int> mSelectMaxNumX;

	Vec2 mSelectCurrentNum;

	// ステージの状態ごとのスケール
	Vec3 mDisabledPanelScale;
	Vec3 mSelectPanelScale;
	Vec3 mPressedPanelScale;

	// 数字番号
	int mNumObjMax;

	Vec2 mNumTexSize;

	TextureKey mHyphenTex;
	std::vector<TextureKey> mNumberTex;
};
RegisterScript(SelectPanel);
