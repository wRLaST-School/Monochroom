#pragma once
#include "IScriptObject.h"

class PlayerGoggle :
	public IScriptObject
{
private:
	bool mIsHavingGoggle = false;
	bool mIsEquipGoggle = false;
	bool mIsGoggleChangeWaiting = false;

public:
	void Init();
	void Update();
	void Draw();
	void CopyComponent(IComponent* src) { src; }

public:
	bool GetIsEquipGoggle() { return mIsEquipGoggle; }
	void SetIsHavingGoggle(bool isHaving);
	bool GetIsHavingGoggle() { return mIsHavingGoggle; }

	DefDel;
};

RegisterScript(PlayerGoggle);

