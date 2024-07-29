#pragma once
#include "IUITab.h"
class SoundTab :
    public IUITab
{
private:

    enum SoundMenuNum
    {
        MASTER, // マスター音量
        BGM,    // BGM音量
        SE,     // SE音量
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
    Vec3 mSoundNoneSelectScale;
    Vec3 mSoundSelectScale;

    // それぞれの項目の変化値
    std::vector<float> mSoundSensivity;

    float mSoundDefuValue;
    float mSoundMaxValue;

    // サウンドの項目数
    int mSoundOptionNum;

    // サウンド項目の現在の項目番号
    int mSoundCurrentNum;

    Object3D* mItemsParentObj;

    // サウンドメニューオブジェクト
    std::vector<UI3DTabItemStatus>mSoundUIObj;

    // 番号のテクスチャ
    std::vector<TextureKey> mNumberTex;
};

RegisterScript(SoundTab);