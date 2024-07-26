#pragma once
#include "IScriptObject.h"

//ステート前方宣言
class IPlayerGoggleState;

class GoggleScr;

//
class PlayerGoggle :
	public IScriptObject
{
private:
	std::unique_ptr<IPlayerGoggleState> mState = nullptr;

	const int32_t kEquipGrayEffectTime = 20;
	const int32_t kTakeOffGrayEffectTime = 13;

public:
	void Init();
	void Update();
	void Draw();
	void CopyComponent(IComponent* src) { src; }

public:
	void GettedGoggle();
	void TakeOnOffGoggle(bool isOn);

	//モデルのゴーグルとスプライトの方の位置を合わせる
	void AlignGoggle2DPos();
	//
	void FinishedEquip();
	void FinishedTakeOff();

	void TakeOffGrayScaleEffect();

public:
	//装着完了したか
	bool GetFinishedEquipGoggle();
	//ゴーグル外し終わったか
	bool GetFinishedTakingOffGoggle();
	//グレースケールの演出が終わったか
	bool GetIsFinishedGrayScaleEffect();

private:
	GoggleScr* GetGoggleScr();

public:
	//ステート
	void ChangeState(std::unique_ptr<IPlayerGoggleState> state);

	DefDel;
};

RegisterScript(PlayerGoggle);

