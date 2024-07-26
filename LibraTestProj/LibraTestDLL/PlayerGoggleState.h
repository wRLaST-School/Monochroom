#pragma once
#include "IScriptObject.h"

//前方宣言
class PlayerGoggle;


//ステート親クラス
class IPlayerGoggleState
{
protected:
	int32_t mTimer = 0;
	PlayerGoggle* mPlayerGoggle = nullptr;

public:
	void SetPlayerGoggleClass(PlayerGoggle* playerGoggle) { mPlayerGoggle = playerGoggle; }

	bool GetTimeRatio(int32_t timeRimit) { return (float)mTimer / (float)timeRimit; }

public:
	virtual void Init() = 0;
	virtual void Update() = 0;
};


//-------------------------------------------------------------------------------------
//ゴーグル持ってない
class PlayerGoggleStateNone : public IPlayerGoggleState
{
private:
	const int32_t kTimeRimit = 30;

public:
	void Init()override;
	void Update()override;
};

//ゴーグル持ってる
class PlayerGoggleStateHaving : public IPlayerGoggleState
{
private:
	const int32_t kTimeRimit = 30;

public:
	void Init()override;
	void Update()override;
};

//ゴーグル装着途中
class PlayerGoggleStateEquiping : public IPlayerGoggleState
{
private:
	const int32_t kTimeRimit = 30;

public:
	void Init()override;
	void Update()override;
};

//ゴーグル装着完了
class PlayerGoggleStateEquiped : public IPlayerGoggleState
{
private:
	const int32_t kTimeRimit = 30;

public:
	void Init()override;
	void Update()override;
};

//ゴーグル演出途中
class PlayerGoggleStateTakingOffEffect : public IPlayerGoggleState
{
private:
	const int32_t kTimeRimit = 30;

public:
	void Init()override;
	void Update()override;
};

//ゴーグル外し途中
class PlayerGoggleStateTakingOff : public IPlayerGoggleState
{
private:
	const int32_t kTimeRimit = 30;

public:
	void Init()override;
	void Update()override;
};