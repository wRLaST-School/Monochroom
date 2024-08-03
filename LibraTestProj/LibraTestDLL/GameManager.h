#pragma once
#include "IScriptObject.h"
#include <Object3D.h>
#include <Camera.h>
#include <StageGenerater.h>
#include <vector>
#include <SuperUI.h>
#include <SelectPanel.h>
#include <AttractParticleManager.h>
#include <GoggleScr.h>


class GameManager :
	public IScriptObject
{
private:
	Object3D* mPlayer;
	Camera* mCamera;
	StageGenerater* mStageGenerater;
	SelectPanel* mSelectPanel;
	SuperUI* mUIScript;
	AttractParticleManager* mAttractParticleManager;

	bool isStop;

public:
	void Awake();
	void Init();
	void Update();
	void Draw();
	void CopyComponent(IComponent* src);

	DefDel;

public:
	Object3D* GetPlayer();
	Camera* GetCamera();
	StageGenerater* GetStageGenerater();
	AttractParticleManager* GetAttractParticleManager();

	FlyBlock* GetFlyBlock(IComponent* parentComp);
	void PlayerGettedGoggle(Object3D* player, GoggleScr* goggle);

	bool GetisStop();

	// ゲーム一時停止の設定
	void SetIsStop(bool IsStop);

	static GameManager* GetInstance();
};

RegisterScript(GameManager);

