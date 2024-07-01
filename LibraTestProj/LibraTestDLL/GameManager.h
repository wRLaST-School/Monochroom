#pragma once
#include "IScriptObject.h"
#include <Object3D.h>
#include <Camera.h>
#include <vector>

class GameManager :
	public IScriptObject
{
private:
	Object3D* mPlayer;
	Camera* mCamera;
	bool isStop;

public:
	void Init();
	void Update();
	void Draw();
	void CopyComponent(IComponent* src);

	DefDel;

public:
	Object3D* GetPlayer();
	Camera* GetCamera();
	bool GetisStop();

	// ゲーム一時停止の設定
	void SetIsStop(bool IsStop);

	static GameManager* GetInstance();
};

RegisterScript(GameManager);

