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
		MOVETOCAPCEL,
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

	// タイトル？の更新処理
	void TitleUpdate();

	// セレクトパネルの更新処理
	void SelectStageUpdate();

	// カプセルに移動の更新処理
	void MoveToCapcelUpdate();

	// ステージチェンジの更新処理
	void StageChangeUpdate();

	void CheckNumOver();

	void InitTex();
private:

	// メインカメラ
	Object3D* mCameraObj;

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

	// セレクトシーンのステート
	SelectState mSelectState;

	// タイトルカメラ移動の配列
	Vec3 mTitleCameraFirstPos;
	Vec3 mTitleCameraFirstRota;

	std::vector<Vec3> mTitleCameraMovePos;
	std::vector<Vec3> mTitleCameraMoveRota;

	// シーンチェンジカメラ移動の配列
	Vec3 mSceneChangeCameraFirstPos;
	Vec3 mSceneChangeCameraFirstRota;

	std::vector<Vec3> mSceneChangeCameraMovePos;
	std::vector<Vec3> mSceneChangeCameraMoveRota;

	bool IsTitleToSelect;
	float mTitleMoveTime;
	float mTitleMoveTimeMax;

	float mSceneChangeCameraTime;
	float mSceneChangeCameraTimeMax;
};
RegisterScript(SelectPanel);
