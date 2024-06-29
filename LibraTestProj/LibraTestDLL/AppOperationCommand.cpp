#include "AppOperationCommand.h"
#include <Input.h>
using namespace Input;

Vec3 AppOperationCommand::PlayerMoveCommand()
{
	float right = Key::Down(DIK_RIGHTARROW) || Key::Down(DIK_D);
	float left = Key::Down(DIK_LEFTARROW) || Key::Down(DIK_A);
	float up = Key::Down(DIK_UPARROW) || Key::Down(DIK_W);
	float down = Key::Down(DIK_DOWNARROW) || Key::Down(DIK_S);

	Vec3 vec = { 0,0,0 };
	vec.x = (right - left) + Pad::GetLStick().x;
	vec.z = (up - down) + Pad::GetLStick().y;

	return vec;
}

bool AppOperationCommand::PlayerJumpCommand()
{
	return Key::Triggered(DIK_SPACE) || Pad::Triggered(Button::A);
}

Vec3 AppOperationCommand::PlayerAngleCommand()
{
	Vec2 mouseMove = Mouse::GetInstance()->GetMouseMove();

	return Vec3(mouseMove.x, mouseMove.y,0);
}

bool AppOperationCommand::PlayerAttractBlockCommand()
{
	return Mouse::Triggered(Click::Left) || Pad::Triggered(Button::X);
}

bool AppOperationCommand::PlayerConfirmCommand()
{
	return Mouse::Triggered(Click::Left) || Pad::Triggered(Button::A);
}

bool AppOperationCommand::UserDebugStopGameCommand()
{
	return Key::Triggered(DIK_F5);
}

AppOperationCommand* AppOperationCommand::GetInstance()
{
	static AppOperationCommand instance;
	return &instance;
}
