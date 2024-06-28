#pragma once
#include <Vec3.h>
#include <Vec2.h>
#include <Float2.h>

class AppOperationCommand
{
public:
	Vec3 PlayerMoveCommand();
	bool PlayerJumpCommand();
	Vec3 PlayerAngleCommand();

	bool PlayerAttractBlockCommand();

	bool UserDebugStopGameCommand();


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

