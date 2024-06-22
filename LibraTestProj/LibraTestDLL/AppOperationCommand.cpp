#include "AppOperationCommand.h"
#include <Input.h>
using namespace Input;

void AppOperationCommand::PlayerMouseAngleUpdate()
{
	mouseMoveVec_ = Vec2(Mouse::GetPos()) - oldMousePos_;

	oldMousePos_ = Mouse::GetPos();
}

void AppOperationCommand::PlayerMouseAngleInit()
{
	oldMousePos_ = Mouse::GetPos();
}

Vec3 AppOperationCommand::PlayerMoveCommand()
{
	bool right = Key::Down(DIK_RIGHTARROW) || Key::Down(DIK_D);
	bool left = Key::Down(DIK_LEFTARROW) || Key::Down(DIK_A);
	bool up = Key::Down(DIK_UPARROW) || Key::Down(DIK_W);
	bool down = Key::Down(DIK_DOWNARROW) || Key::Down(DIK_S);

	Vec3 vec = { 0,0,0 };
	vec.x = (right - left) || Pad::GetLStick().x;
	vec.z = (up - down) || Pad::GetLStick().y;

	return vec;
}

bool AppOperationCommand::PlayerJumpCommand()
{
	return Key::Triggered(DIK_SPACE) || Pad::Triggered(Button::A);
}

Vec3 AppOperationCommand::PlayerAngleCommand()
{
	return Vec3(mouseMoveVec_);
}

AppOperationCommand* AppOperationCommand::GetInstance()
{
	AppOperationCommand instance;
	return &instance;
}
