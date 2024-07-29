#pragma once
#include "IScriptObject.h"
#include <Object3D.h>
#include <Camera.h>
#include <StageGenerater.h>
#include <vector>
#include "SelectPanel.h"
#include "SuperUI.h"

class GameManager :
	public IScriptObject
{
private:
	Object3D* mPlayer;
	Camera* mCamera;
	StageGenerater* mStageGenerater;
	SelectPanel* mSelectPanel;
	SuperUI* mUIScript;

	bool isStop;
	static bool mIsChangeScene;

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
	
	bool GetisStop();

	// ゲーム一時停止の設定
	void SetIsStop(bool IsStop);

	static GameManager* GetInstance();
};

RegisterScript(GameManager);

