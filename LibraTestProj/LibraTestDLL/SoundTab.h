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

    void OnUpdate()  override;
    void OffUpdate()  override;

private:

    // サウンドの項目数
    int mSoundOptionNum;

    // サウンド項目の現在の項目番号
    int mCurrentNum;

    // サウンドメニューオブジェクト
    std::vector<UI3DTabItemStatus>mSoundUIObj;
};

