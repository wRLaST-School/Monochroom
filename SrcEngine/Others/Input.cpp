#include "Input.h"
#include "Essentials.h"
#include "SpWindow.h"
#include "Util.h"

#include <SpImGui.h>

using namespace Input;

void Key::Init()
{
	Key* instance = GetInstance();
	DirectInput8Create(GetSpWindow()->w.hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&instance->dinput_, nullptr);

	instance->dinput_->CreateDevice(GUID_SysKeyboard, &instance->devkeyboard_, NULL);

	instance->devkeyboard_->SetDataFormat(&c_dfDIKeyboard);

	instance->devkeyboard_->SetCooperativeLevel(GetSpWindow()->hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
}

void Key::Update()
{
	if (ImGui::IsAnyItemActive() || ImGuizmo::IsUsing()) return;

	Key* instance = GetInstance();
	for (size_t i = 0; i < 256; i++)
	{
		instance->prevKeys_[i] = instance->keys_[i];
	}

	instance->devkeyboard_->Acquire();
	instance->devkeyboard_->GetDeviceState(sizeof(instance->keys_), instance->keys_);
}

void Key::Close()
{
	Key* instance = GetInstance();
	instance->dinput_->Release();
	instance->devkeyboard_->Release();
}

bool Key::Down(int32_t key)
{
	if (key >= 256 || key < 0) return false;
	return (bool)GetInstance()->keys_[key];
}

bool Key::Released(int32_t key)
{
	if (key >= 256 || key < 0) return false;
	return (bool)(!GetInstance()->keys_[key] && GetInstance()->prevKeys_[key]);
}

bool Key::Triggered(int32_t key)
{
	if (key >= 256 || key < 0) return false;
	return (bool)(GetInstance()->keys_[key] && !GetInstance()->prevKeys_[key]);
}

bool Input::Key::DoubleTriggered(int32_t key)
{
	if (key >= 256 || key < 0) return false;

	// 一回押したら
	if (Triggered(key))
	{
		GetInstance()->doubleKeys_[key].isPush = true;
		GetInstance()->doubleKeys_[key].pushKey = key;
	}

	if (GetInstance()->doubleKeys_[key].isPush)
	{
		// フレーム内に押してないなら
		if (GetInstance()->doubleKeys_[key].frame >= GetInstance()->doubleKeys_[key].maxFrame)
		{
			// 初期値に戻しfalseを返す
			GetInstance()->doubleKeys_[key] = Double();
			return false;
		}
		// 1フレーム以上の時に判定取る(同じフレームだと100%trueになる)
		else if (GetInstance()->doubleKeys_[key].frame >= 1)
		{
			// もう一回押したら
			if (Triggered(GetInstance()->doubleKeys_[key].pushKey))
			{
				// 初期値に戻しtrueを返す
				GetInstance()->doubleKeys_[key] = Double();
				return true;
			}
		}

		GetInstance()->doubleKeys_[key].frame++;
	}

	return false;
}

Key* Key::GetInstance()
{
	static Key obj;
	return &obj;
}

void Input::Pad::Init()
{

}

void Input::Pad::Update()
{
	Input::Pad* instance = GetInstance();

	instance->lastPadState_ = instance->padState_;
	instance->lastTriggerState_[0] = instance->triggerState_[0];
	instance->lastTriggerState_[1] = instance->triggerState_[1];

	instance->connected = XInputGetState(instance->gamepadIndex, &instance->padState_) == ERROR_SUCCESS;
	instance->triggerState_[0] = instance->padState_.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
	instance->triggerState_[1] = instance->padState_.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
}

void Input::Pad::Close()
{
	//XInputEnable(false);
}

bool Input::Pad::Down(const Button& button)
{
	return GetInstance()->padState_.Gamepad.wButtons & (UINT)button;
}

bool Input::Pad::Released(const Button& button)
{
	return !(GetInstance()->padState_.Gamepad.wButtons & (UINT)button) && GetInstance()->lastPadState_.Gamepad.wButtons & (UINT)button;
}

bool Input::Pad::Triggered(const Button& button)
{
	return !(GetInstance()->lastPadState_.Gamepad.wButtons & (UINT)button) && GetInstance()->padState_.Gamepad.wButtons & (UINT)button;
}

bool Input::Pad::Down(const Trigger& side)
{
	return GetInstance()->triggerState_[(int32_t)side];
}

bool Input::Pad::Released(const Trigger& side)
{
	return !(GetInstance()->triggerState_[(int32_t)side]) && GetInstance()->lastTriggerState_[(int32_t)side];
}

bool Input::Pad::Triggered(const Trigger& side)
{
	return GetInstance()->triggerState_[(int32_t)side] && !(GetInstance()->lastTriggerState_[(int32_t)side]);
}

Pad* Input::Pad::GetInstance()
{
	static Pad obj;
	return &obj;
}

Input::Pad::Stick Input::Pad::GetLStick()
{
	Input::Pad* instance = GetInstance();
	short lx = Util::Clamp(instance->padState_.Gamepad.sThumbLX, (short)-32767, (short)32767);
	short ly = Util::Clamp(instance->padState_.Gamepad.sThumbLY, (short)-32767, (short)32767);

	if (Vec2(lx, ly).GetSquaredLength() <= (instance->deadZone_ * 32.767f) * (instance->deadZone_ * 32.767f))
		return Stick{ 0,0 };

	return Stick{ (float)lx / 32767.f, (float)ly / 32767.f };
}

Input::Pad::Stick Input::Pad::GetRStick()
{
	Input::Pad* instance = GetInstance();
	short rx = Util::Clamp(instance->padState_.Gamepad.sThumbRX, (short)-32767, (short)32767);
	short ry = Util::Clamp(instance->padState_.Gamepad.sThumbRY, (short)-32767, (short)32767);

	if (Vec2(rx, ry).GetSquaredLength() <= (instance->deadZone_ * 32.767f) * (instance->deadZone_ * 32.767f))
		return Stick{ 0,0 };

	return Stick{ (float)rx / 32767.f, (float)ry / 32767.f };
}

void Input::Pad::SetDeadZone(float range)
{
	GetInstance()->deadZone_ = Util::Clamp(range, 0.0f, 100.0f);
}

float Input::Pad::GetDeadZone()
{
	return GetInstance()->deadZone_;
}

bool Input::Pad::IsConnected()
{
	return GetInstance()->connected;
}

void Input::Mouse::Init()
{
	Mouse* instance = GetInstance();
	GetDInput()->CreateDevice(GUID_SysMouse, &instance->devmouse_, NULL);
	instance->devmouse_->SetDataFormat(&c_dfDIMouse);

	instance->devmouse_->SetCooperativeLevel(GetSpWindow()->hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
}

void Input::Mouse::Update()
{
	if (ImGui::IsAnyItemHovered() || ImGuizmo::IsUsing()) return;

	Mouse* ins = GetInstance();
	ins->prevState_ = ins->state_;

	ins->oldCursor_ = ins->cursor_;

	ins->devmouse_->Acquire();
	ins->devmouse_->Poll();
	ins->devmouse_->GetDeviceState(sizeof(DIMOUSESTATE), &ins->state_);

	POINT pos;
	GetCursorPos(&pos);

	ScreenToClient(GetSpWindow()->hwnd, &pos);

	ins->cursor_ = { (float)pos.x, (float)pos.y };

	ins->cusorMove_ = {
		ins->cursor_.x - ins->oldCursor_.x,
		ins->cursor_.y - ins->oldCursor_.y
	};
}

void Input::Mouse::Close()
{
	Mouse* ins = GetInstance();
	ins->devmouse_->Release();
}

bool Input::Mouse::Down(const Click& b)
{
	return GetInstance()->state_.rgbButtons[(int32_t)b] & (0x80);
}

bool Input::Mouse::Triggered(const Click& b)
{
	return (!(GetInstance()->prevState_.rgbButtons[(int32_t)b] & (0x80))) && (GetInstance()->state_.rgbButtons[(int32_t)b] & (0x80));
}

bool Input::Mouse::Released(const Click& b)
{
	return (!(GetInstance()->state_.rgbButtons[(int32_t)b] & (0x80))) && (GetInstance()->prevState_.rgbButtons[(int32_t)b] & (0x80));
}

Float2 Input::Mouse::GetVel()
{
	return { (float)GetInstance()->state_.lX , (float)GetInstance()->state_.lY };
}

Float2 Input::Mouse::GetPos()
{
	return GetInstance()->cursor_;
}

Float2 Input::Mouse::GetMouseMove()
{
	return GetInstance()->cusorMove_;
}

float Input::Mouse::GetWheelVel()
{
	return (float)GetInstance()->state_.lZ;
}

bool Input::Mouse::GetShowCursor()
{
	return GetInstance()->showCursor;
}

Mouse* Input::Mouse::GetInstance()
{
	static Mouse obj;
	return &obj;
}

void Input::Mouse::ShowCursorM()
{
	if (!GetInstance()->showCursor) {
		while (ShowCursor(TRUE) < 0) {};
		GetInstance()->showCursor = true;
	}
}

void Input::Mouse::HideCursor()
{
	if (GetInstance()->showCursor) {
		while (ShowCursor(FALSE) >= 0) {};
		GetInstance()->showCursor = false;
	}
}

void Input::Mouse::SetPos(Float2 pos)
{
	SetCursorPos((int)pos.x, (int)pos.y);
}

IDirectInput8* Input::GetDInput()
{
	return Key::GetInstance()->dinput_;
}
