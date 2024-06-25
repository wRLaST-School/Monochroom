#pragma once
#include "IScriptObject.h"
#include <Object3D.h>
#include <vector>

class GameManager :
	public IScriptObject
{
private:
	Object3D* mPlayer;
	bool isStop;

public:
	void Init();
	void Update();
	void Draw();
	void CopyComponent(IComponent* src);

	DefDel;

public:
	Object3D* GetPlayer();
	bool GetisStop();

	static GameManager* GetInstance();
};

RegisterScript(GameManager);

