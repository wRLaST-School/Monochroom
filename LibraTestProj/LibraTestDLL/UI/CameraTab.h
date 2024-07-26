#pragma once
#include "IUITab.h"

class CameraTab :
    public IUITab
{
private:

    enum CameraMenuNum
    {
        MOUSESENSITIVITY_X,
        MOUSESENSITIVITY_Y,
        PADSENSITIVITY_X,
        PADSENSITIVITY_Y,
    };
public:
    void Init()  override;
    void Update()  override;
    void Draw()  override;
    void CopyComponent(IComponent* src) override;

    void MenuUpdate()override;

    void OnUpdate()  override;
    void OffUpdate()  override;

private:

    // カメラの選択されてないスケール
    Vec3 mCameraNoneSelectScale;
    Vec3 mCameraSelectScale;

    // それぞれの項目の変化値
    std::vector<float> mCameraSensivity;

    Vec2 mMouseSensivity = { 1,1 };
    Vec2 mPadSensivity = { 1,1 };

    float mCameraDefuValue;
    float mCameraMaxValue;

    // カメラの項目数
    int mCameraOptionNum;

    // カメラ項目の現在の項目番号
    int mCameraCurrentNum;

    std::unique_ptr<Object3D> mItemsParentObj;
    // 番号のテクスチャ
    std::vector<TextureKey> mNumberTex;
    // カメラメニューオブジェクト
    std::vector<UI3DTabItemStatus>mCameraUIObj;


public:
    Vec2 GetMouseSensivity();

    Vec2 GetPadSensivity();
};

RegisterScript(CameraTab);
