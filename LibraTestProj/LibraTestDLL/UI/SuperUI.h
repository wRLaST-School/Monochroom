#pragma once
#include "IScriptObject.h"
#include <Object3D.h>
#include "Easing.h"
#include <Color.h>

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

    enum UIOptionNum
    {
        CAMERA,// カメラ
        GUID,// 説明画面
        QUIT_TITLE,// タイトルへ戻る
    };

    enum UIMenuState 
    {
        DISABLED,// 選択されていない状態
        SELECT,// 選択されている状態
        PRESSED,// 押された後の状態
    };

    struct UIMenuStatus
    {
        // ベースの座標
        Vec2 afterBasePos;

        // 移動前の座標
        Vec2 beforePos;

        // 出現時に変化する座標量
        Vec2 posChangeValue;

        // 選択された時に変化するスケール量
        Vec2 scaleChangeValue;

        // 現在のカラー
        Color correntColor;

        // 今アクティブ状態かどうか
        bool IsActiveMenu;

        // タイミングがずれた時
        bool IsTimingOff;

        // UIのオプションのそれぞれの状態
        UIMenuState state;
    };

private:
    // メニューのステート

    // 親子関係の設定
    void LoadTexInit();

    // メニューが開いた瞬間のリセット
    void MomentMenuReset();

    // オプションのリセット
    void ResetOption();

    // UIの項目の更新処理
    void UIOptionsUpdate();

private:// メンバー変数

    // UIメニューを開くかどうか
    bool mIsOpenUIMenu;

    // UIメニューの開いた瞬間
    bool mIsMomentOpenMenu;

    // UIが表示するかどうか
    bool mIsDisplayUI;

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

    // UIの項目の全体共通の初期座標
    float mUICommonBasePos;

    float mUICommonSpacePos;

    // UIの現在の項目をNUM型に保存
    int mUICurrentNum;

    // UIの項目の選択時のそれぞれのスケール変化量
    Vec2 mUIDesabledScale;
    Vec2 mUISelectScale;

    // UIの項目の選択時のそれぞれのカラー変化量
    Color mDesabledColor;
    Color mSelectColor;
    Color mPressedColor;

    // UIのオプション項目
    Object3D* mUIOptionParentObj;
    std::vector<Object3D*> mUIOptionObjs;

    // UIのオプションのそれぞれの項目
    Object3D* mUIOptionItemParentObj;
    std::vector<Object3D*> mUIOptionItemObjs;


};
RegisterScript(SuperUI);

