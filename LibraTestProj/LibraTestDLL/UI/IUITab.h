#pragma once
#include <Color.h>
#include <Object3D.h>

class IUITab
{
protected:
    enum UIMenuState
    {
        DISABLED,   // 選択されていない状態
        SELECT,     // 選択されている状態
        PRESSED,    // 押された後の状態
    };

    struct UI3DTabItemStatus
    {
        Object3D* planeObj;

        Object3D* numObj;

        bool IsActive;

        // UIのオプションのそれぞれの状態
        UIMenuState state;

        // 選択された時に変化するスケール量
        Vec3 scaleChangeValue;

        // 現在のカラー
        Color correntColor;

        // 現在のボタンカラー
        Color buttonColor;

        // 現在ンのメニュー内部のテキストカラー
        Color textCurrentColor;
    };

public:
	virtual void Init() {};
	virtual void Update() {};
	virtual void Draw() {};

	virtual void OnUpdate() {};
	virtual void OffUpdate() {};
};
