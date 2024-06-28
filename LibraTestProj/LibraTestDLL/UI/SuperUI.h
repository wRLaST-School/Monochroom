#pragma once
#include "IScriptObject.h"
#include <Object3D.h>
#include "Easing.h"
#include <Color.h>
#include "PlayerControl.h"

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
        CAMERA,     // カメラ
        GRAPHICS,   // グラフィックス
        SOUND,      // サウンド
    };

    enum MainMenuNum
    {
        OPTIONS,        // オプション
        GUID,           // ガイド
        QUIT_SELECT,    // セレクトへ戻る
        QUIT_TITLE,     // タイトルへ戻る
    };

    enum CameraMenuNum
    {
        MOUSESENSITIVITY_X,
        MOUSESENSITIVITY_Y,
    };

    enum GraphicsMenuNum
    {
        POSTPROCESS,    // ポストプロセス
        SHADOW,         // 影
    };

    enum SoundMenuNum
    {
        MASTER, // マスター音量
        BGM,    // BGM音量
        SE,     // SE音量
    };

    enum UIMenuState 
    {
        DISABLED,   // 選択されていない状態
        SELECT,     // 選択されている状態
        PRESSED,    // 押された後の状態
    };





    struct UI3DTabItemStatus
    {
        Object3D* planeObj;

        bool IsActive;

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

        // 現在のボタンカラー
        Color buttonColor;

        // 現在ンのメニュー内部のテキストカラー
        Color textCurrentColor;

        // 今アクティブ状態かどうか
        bool IsActiveMenu;

        // メニューを選択後に文字が徐々の演出フラグ
        bool IsPressedAlpha;

        // タイミングがずれた時
        bool IsTimingOff;

        // UIのオプションのそれぞれの状態
        UIMenuState state;
    };

    struct UIInternalItem
    {
        Vec2 basePos;

        Vec2 scaleChageValue;

        bool IsActive;
    };

private:
    // メニューのステート

    // UIのオブジェクト3Dの初期設定
    void UIObj3DInit();

    // 親子関係の設定
    void LoadTexInit();

    // メニューが開いた瞬間のリセット
    void MomentMenuReset();

    // オプションのリセット
    void ResetOption();

    // UIの項目の更新処理
    void UIOptionsUpdate();

    // カメラメニューの更新処理
    void CameraMenuUpdate();

    // カメラメニューの描画
    void CameraMenuDraw();

    // ガイドメニューの描画
    void GuidMenuDraw();

    // 戻るメニューの描画
    void QuitMenuDraw();

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
    Easing mTextAlphaEase;

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

    Color mDesabledTextColor;
    Color mSelectTextColor;
    Color mPressedTextColor;

    // UIのメニューテキストのパラメータ
    Vec2 mUIMenuTextPos;
    Vec2 mUIMenuTexScale;

    // UIのボードのパラメータ
    Vec2 mUIBoardPos;
    Vec2 mUIBoardScale;
    Color mUIBoardCurrentColor;

    // UIのメニュー内部のテキストの色
    Color mUITextBeforeColor;
    Color mUITextAfterColor;

    // UIのメニュー内部のテキストパラメータ
    Vec2 mUITextBasePos;

    // カメラメニューの感度のパラメータ
    Vec2 mCameraSensitivity;

    // カメラの内部項目
    int mNumCameraItem;
    std::vector<UIInternalItem> mCameraItems;

    // メニューの項目数
    int mNumMenu;

    // メニューのオプションのタブの項目数
    int mNumOption;

    // カメラの項目数
    int mCameraOptionNum;

    // グラフィックスの項目数
    int mGraphicsOptionNum;

    // サウンドの項目数
    int mSoundOptionNum;


    // UIの全体の親オブジェクト
    Object3D* mMenuParentObj;

    // UIのメインメニューの項目オブジェクト
    std::vector<UI3DTabItemStatus*>mMenuUIObj;

    // UIのタブメニュー項目オブジェクト
    std::vector<UI3DTabItemStatus*>mMenuTabUIObj;



    // UIのオプション画面の項目オブジェクト
    std::vector<UI3DTabItemStatus*>mCameraUIObj;
    std::vector<UI3DTabItemStatus*>mGraphicUIObj;
    std::vector<UI3DTabItemStatus*>mSoundUIObj;



    // プレイヤーコントロール(取得用)
    PlayerControl* mPlayerControl;


};
RegisterScript(SuperUI);

