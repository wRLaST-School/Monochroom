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
    void CopyComponent(IComponent* src) override;

    void MenuUpdate()override;

    void OnUpdate()  override;
    void OffUpdate()  override;

private:

    // サウンドの選択されてないスケール
    Vec3 mGraphicsNoneSelectScale;
    Vec3 mGraphicsSelectScale;

    // それぞれの項目の変化値
    std::vector<bool> mGraphicsFlags;

    float mGraphicsDefuValue;
    float mGraphicsMaxValue;

    // グラフィックスの項目数
    int mGraphicsOptionNum;

    // グラフィックス項目の現在の項目番号
    int mGraphicsCurrentNum;

    std::unique_ptr<Object3D> mItemsParentObj;

    std::vector<Object3D*> mOnPlaneObj;
    std::vector<Object3D*> mOffPlaneObj;

    Color mSelectColor;
    Color mDisabledColor;


    // グラフィックスメニューオブジェクト
    std::vector<UI3DTabItemStatus>mGraphicsUIObj;
};
RegisterScript(GraphicsTab);
