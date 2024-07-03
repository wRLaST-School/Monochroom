#pragma once
#include "IUITab.h"
class GraphicsTab :
    public IUITab
{
private:

    enum GraphicsMenuNum
    {
        POSTPROCESS,    // ポストプロセス
        SHADOW,         // 影
    };

public:
    void Init()  override;
    void Update()  override;
    void Draw()  override;

    void OnUpdate()  override;
    void OffUpdate()  override;

private:

    // グラフィックスの項目数
    int mGraphicsOptionNum;

    // グラフィックス項目の現在の項目番号
    int mCurrentNum;

    // グラフィックスメニューオブジェクト
    std::vector<UI3DTabItemStatus>mGraphicUIObj;
};

