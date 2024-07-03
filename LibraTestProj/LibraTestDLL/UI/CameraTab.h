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
    };
public:
    void Init()  override;
    void Update()  override;
    void Draw()  override;

    void OnUpdate()  override;
    void OffUpdate()  override;

private:

    // カメラの項目数
    int mCameraOptionNum;

    // カメラ項目の現在の項目番号
    int mCurrentNum;

    // カメラメニューオブジェクト
    std::vector<UI3DTabItemStatus>mCameraUIObj;

};

