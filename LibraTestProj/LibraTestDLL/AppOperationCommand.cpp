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
	Vec2 mouseMove = Mouse::GetInstance()->GetVel();

	return Vec3(mouseMove.x, mouseMove.y,0);
}

bool AppOperationCommand::PlayerAttractBlockCommand()
{
	return Mouse::Triggered(Click::Left) || Pad::Triggered(Button::X);
}

bool AppOperationCommand::PlayerGetGoggleCommand()
{
	return Mouse::Triggered(Click::Left) || Pad::Triggered(Button::X);
}

bool AppOperationCommand::PlayerEquipGoggleCommand()
{
	return Mouse::Triggered(Click::Middle) /*||*/;
}

bool AppOperationCommand::PlayerConfirmCommand()
{
	return Mouse::Triggered(Click::Left) || Pad::Triggered(Button::A);
}

bool AppOperationCommand::ReStartCommand()
{
	return Key::Triggered(DIK_R)||Pad::Triggered(Button::Back);
}

bool AppOperationCommand::UserDebugStopGameCommand()
{
	return Key::Triggered(DIK_F5);
}

bool AppOperationCommand::UISelectUpCommand()
{
	return Key::Triggered(DIK_W) || Pad::Triggered(Button::Up);
}

bool AppOperationCommand::UISelectDownCommand()
{
	return Key::Triggered(DIK_S) || Pad::Triggered(Button::Down);
}

bool AppOperationCommand::UISelectLeftCommand()
{
	return Key::Triggered(DIK_A) || Pad::Triggered(Button::Left);
}

bool AppOperationCommand::UISelectRightCommand()
{
	return Key::Triggered(DIK_D) || Pad::Triggered(Button::Right);
}

bool AppOperationCommand::UIDicisionCommand()
{
	return Key::Triggered(DIK_SPACE) || Pad::Triggered(Button::A);
}

bool AppOperationCommand::UITabLeftCommand()
{
	return Key::Triggered(DIK_Q) || Pad::Triggered(Button::L);
}

bool AppOperationCommand::UITabRightCommand()
{
	return Key::Triggered(DIK_E) || Pad::Triggered(Button::R);
}

bool AppOperationCommand::UIBackCommand()
{
	return Key::Triggered(DIK_ESCAPE) || Pad::Triggered(Button::Start);
}

AppOperationCommand* AppOperationCommand::GetInstance()
{
	static AppOperationCommand instance;
	return &instance;
}
