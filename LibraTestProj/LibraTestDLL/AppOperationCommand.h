#pragma once
#include <Vec3.h>

class AppOperationCommand
{
public:
	Vec3 PlayerMoveCommand();
	bool PlayerJumpCommand();


public:
	static AppOperationCommand* GetInstance();

private:
	// singleton
	AppOperationCommand(){}
	~AppOperationCommand() = default;
	AppOperationCommand(const AppOperationCommand&) = delete;
	AppOperationCommand(AppOperationCommand&&) = delete;
	AppOperationCommand& operator=(const AppOperationCommand&) = delete;
	AppOperationCommand& operator=(AppOperationCommand&&) = delete;

};

