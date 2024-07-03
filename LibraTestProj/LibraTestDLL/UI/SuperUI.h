#pragma once
#include "IScriptObject.h"
#include <Object3D.h>
#include "Easing.h"
#include <Color.h>
#include "PlayerControl.h"
#include "IUITab.h"
#include "CameraTab.h"
#include "GraphicsTab.h"
#include "SoundTab.h"

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
        GUID,           // ガイド
        OPTIONS,        // オプション
        QUIT_TITLE,     // タイトルへ戻る
        QUIT_SELECT,    // セレクトへ戻る
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

    // UIのオブジェクト3Dの更新処理
    void UIObj3DUpdate();

    // 親子関係の設定
    void LoadTexInit();

    // メニューが開いた瞬間のリセット
    void MomentMenuReset();

    // オプションのリセット
    void ResetOption();

    // UIの項目の更新処理
    void UIOptionsUpdate();

    // UIのメインメニューの更新処理
    void UIMainMenuUpdate();

    // UIのタブメニューの更新処理
    void UITabMenuUpdate();

    // カメラメニューの更新処理
    void CameraMenuUpdate();

    // カメラメニューの描画
    void CameraMenuDraw();

    // ガイドメニューの描画
    void GuidMenuDraw();

    // 戻るメニューの描画
    void QuitMenuDraw();

    // UIのメニューオンにした時の初期化
    void UIMainMenuOnReset();

    // UIのメニューオンにした時の初期化
    void UIMainMenuOffReset();

    // UIのタブメニューオンにした時の初期化
    void UITabMenuOn();

    // UIのタブメニューオフにしたときの初期化
    void UITabMenuOff();

    // カメラメニューの時の初期化
    void UICameraTabOn();

private:// メンバー変数

    // UIメニューを開くかどうか
    bool mIsOpenUIMenu;

    // UIメニューの開いた瞬間
    bool mIsMomentOpenMenu;

    // UIが表示するかどうか
    bool mIsDisplayUI;

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

    // カメラメニューの感度のパラメータ
    Vec2 mCameraSensitivity;

    // メニューの項目数
    int mNumMenu;

    // メニューのオプションのタブの項目数
    int mNumOption;

    // UIの全体の親オブジェクト
    std::unique_ptr<Object3D> mMenuParentObj;

    std::unique_ptr<Object3D> mMenuPlaneObj;

    std::unique_ptr<Object3D> mPlanesParentObj;

    // UIのメインメニューの項目オブジェクト
    std::vector<UI3DTabItemStatus>mMenuUIObj;

    std::unique_ptr<Object3D> mTabsParentObj;

    std::unique_ptr<Object3D> mTabBoardObj;

    // UIのタブメニュー項目オブジェクト
    std::vector<UI3DTabItemStatus>mMenuTabUIObj;

    // UIタブ項目内のオブジェクト
    std::vector<IUITab> mTabItems;

    std::unique_ptr<CameraTab> mCameraItem;
    std::unique_ptr<GraphicsTab> mGpraphicsItem;
    std::unique_ptr<SoundTab> mSoundItem;


    // UIのメニュー項目にアクセスしたときの演出
    bool IsActiveOption;

    // UIのタブのイージング
    Easing mUITabEase;
    Easing mUITabBoardEase;

    // UIイージングパラメータ
    float mTabRotaAfter;
    float mTabRotaBefore;

    float mTabBoardRotaAfter;
    float mTabBoardRotaBefore;

    float mTabEaseTimeLimit;

    // UIのタブのオンフラグ
    bool IsUITabOn;

    // 現在のタブのメニュー番号
    int mCurrentTabNum;

    // プレイヤーコントロール(取得用)
    PlayerControl* mPlayerControl;


};
RegisterScript(SuperUI);

