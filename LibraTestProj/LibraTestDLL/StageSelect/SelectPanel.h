#pragma once
#include "IScriptObject.h"
#include <Object3D.h>
#include <Vec2.h>
#include <Vec3.h>
#include "Easing.h"

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

	enum TitleState
	{
		ROGO,
		NORMAL,
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

	float DegreeToRadian(float angle);

	Vec3 DegreeToRadianVec3(Vec3& angle);

	float Sin_ZeroToOne(float defuValue, float maxCount, float nowCount, float swingWidth);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="defuValue">基準となるもの</param>
	/// <param name="maxCount">最大に到達するまでの時間</param>
	/// <param name="nowCount">今の時間</param>
	/// <param name="swingWidth">ふり幅</param>
	/// <returns></returns>
	Vec3 Sin_ZeroToOne(Vec3 defuValue, float maxCount, float nowCount, Vec3 swingWidth);

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

	// タイトルフォントオブジェクト
	std::unique_ptr<Object3D> mTitleTextObj;

	// タイトルロゴオブジェクト
	std::unique_ptr<Object3D> mTitleRogoObj;

	// タイトル用のゴーグルオブジェクト
	std::unique_ptr<Object3D> mGoggleObj;

	// カプセルのオブジェクト
	std::unique_ptr<Object3D> mCapsuleObj;

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

	TitleState mTitleState;

	bool IsTitleToSelect;
	float mTitleMoveTime;
	float mTitleMoveTimeMax;

	float mSceneChangeCameraTime;
	float mSceneChangeCameraTimeMax;

	float mTitleTextSinDefuPosY;
	float mTitleTextSinSwingPosY;

	float mGoggleSinDefuPosY;
	float mGoggleSinSwingPosY;

	float mGoggleSinDefuRotaZ;
	float mGoggleSinSwingRotaZ;

	float mTitleSinTimer;
	float mTitleSinTimeMax;

	Easing mEaseAlpha;
	bool IsAlphaOn;

	Easing mEaseCapsule;
	float mCapsuleDefuY;
	float mCapsuleEndY;
};
RegisterScript(SelectPanel);
