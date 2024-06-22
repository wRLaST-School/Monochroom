#pragma once
#include <Vec3.h>
#include <Vec2.h>

class AppOperationCommand
{
private:
	Vec2 oldMousePos_ = { 0,0 };
	Vec2 mouseMoveVec_ = { 0,0 };


public:
	void PlayerMouseAngleUpdate();
	void PlayerMouseAngleInit();

public:
	Vec3 PlayerMoveCommand();
	bool PlayerJumpCommand();
	Vec3 PlayerAngleCommand();


public:
	static AppOperationCommand* GetInstance();

private:
	// singleton
	AppOperationCommand() { ; }
	~AppOperationCommand() = default;
	AppOperationCommand(const AppOperationCommand&) = delete;
	AppOperationCommand(AppOperationCommand&&) = delete;
	AppOperationCommand& operator=(const AppOperationCommand&) = delete;
	AppOperationCommand& operator=(AppOperationCommand&&) = delete;

};

