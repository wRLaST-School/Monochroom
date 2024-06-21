#include "AppOperationCommand.h"
#include <Input.h>

Vec3 AppOperationCommand::PlayerMoveCommand()
{
	bool right = Input::Key::Down(DIK_RIGHTARROW) || Input::Key::Down(DIK_D);
	bool left = Input::Key::Down(DIK_LEFTARROW) || Input::Key::Down(DIK_A);
	bool up = Input::Key::Down(DIK_UPARROW) || Input::Key::Down(DIK_W);
	bool down = Input::Key::Down(DIK_DOWNARROW) || Input::Key::Down(DIK_S);

	Vec3 vec = Vec3();
	vec.x = right - left;
	vec.z = up - down;

	return vec;
}

bool AppOperationCommand::PlayerJumpCommand()
{
	return Input::Key::Triggered(DIK_SPACE);
}

AppOperationCommand* AppOperationCommand::GetInstance()
{
    AppOperationCommand instance;
    return &instance;
}
