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
	bool PlayerGetGoggleCommand();

	bool PlayerEquipGoggleCommand();

	bool PlayerConfirmCommand();

	bool ReStartCommand();

	bool UserDebugStopGameCommand();

	// UIのコマンド
	bool UISelectUpCommand();

	bool UISelectDownCommand();
	bool UISelectLeftCommand();
	bool UISelectRightCommand();

	bool UIDicisionCommand();

	bool UITabLeftCommand();
	bool UITabRightCommand();

	bool UIBackCommand();

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

