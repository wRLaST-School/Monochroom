#pragma once
#include "IScriptObject.h"
#include <Object3D.h>
#include "Easing.h"

class SuperUI :
    public IScriptObject
{
public:
    void Init();
    void Update();
    void Draw();
    void CopyComponent(IComponent* src);

    DefDel;

private:// クラス内構造体

    struct UIMenuStatus
    {
        // ベースの座標
        Vec2 basePos;

        // 出現時に変化するスケール量
        Vec2 scaleChangeValue;

        // 今アクティブ状態かどうか
        bool IsActiveMenu;

        // タイミングがずれた時
        bool IsTimingOff;
    };

private:
    // メニューのステート

    // 親子関係の設定
    void LoadTexInit();

    // メニューが開いた瞬間のリセット
    void MomentMenuReset();

private:// メンバー変数

    // UIメニューを開くかどうか
    bool mIsOpenUIMenu;

    // UIメニューの開いた瞬間
    bool mIsMomentOpenMenu;

    // オプションメイン項目の親座標
    Vec2 mUiOptionBasePos;

    // オプションそれぞれの項目の親座標
    Vec2 mUiOptionItemBasePos;

    // イージング
    std::vector<Easing> mEaseUIMenu;

    // UIの項目数
    int32_t mNumOfUIOption;

    // UIのメインメニュー項目
    std::vector<Object3D*>mUIMainMenuObjs;

    // UIの変化するスケール配列
    std::vector<UIMenuStatus> mUIStatus;

    int mUiWaitTimeInterval = 20;
    int mUiWaitTimer = 0;

    // UIのオプション項目
    Object3D* mUIOptionParentObj;
    std::vector<Object3D*> mUIOptionObjs;

    // UIのオプションのそれぞれの項目
    Object3D* mUIOptionItemParentObj;
    std::vector<Object3D*> mUIOptionItemObjs;


};
RegisterScript(SuperUI);

