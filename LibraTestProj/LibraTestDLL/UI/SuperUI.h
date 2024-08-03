#pragma once
#include "IScriptObject.h"
#include <Object3D.h>
#include "Easing.h"
#include <Color.h>
#include "IUITab.h"
#include "CameraTab.h"
#include "GraphicsTab.h"
#include "SoundTab.h"
#include "Timer.h"
#include "SpriteObject.h"

class PlayerControl;

class SuperUI :
    public IScriptObject
{
public:
    void Init();
    void Update();
    void Draw();
    void CopyComponent(IComponent* src);

    // タイトルへ戻るフラグを取得
    bool GetBackToTitle();

    DefDel;

private:// クラス内構造体

    enum UIOptionNum
    {
        CAMERA,     // カメラ
        SOUND,      // サウンド
    };

    enum MainMenuNum
    {
        GUID,           // ガイド
        OPTIONS,        // オプション
        QUIT_TITLE,     // タイトルへ戻る
    };

    enum UIMenuState 
    {
        DISABLED,   // 選択されていない状態
        SELECT,     // 選択されている状態
        PRESSED,    // 押された後の状態
    };

    enum UIQuitTitle
    {
        Yes,
        No,
    };

    enum UIMainMenu
    {
        BackBlack,
        MenuPlane,
        MenuText,
        GuidPlane,
        GuidText,
        OptionPlane,
        OptionText,
        QuitTitlePlane,
        QuitTitleText,
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

    // UIの開いた瞬間の更新処理
    void UIMainMenuMomentUpdate();

    // UIのメインメニューの更新処理
    void UIMainMenuUpdate();

    // UIのタブメニューの更新処理
    void UITabMenuUpdate();

    // ガイドメニューの更新処理
    void UIGuideMenuUpdate();

    // UIのタイトルへ戻るメニューの更新処理
    void UITitleMenuUpdate();

    // UIのメニューオンにした時の初期化
    void UIMainMenuOnReset();

    // UIのメニューオンにした時の初期化
    void UIMainMenuOffReset();

    // UIのタブメニューオンにした時の初期化
    void UITabMenuOn();

    // UIのタブメニューオフにしたときの初期化
    void UITabMenuOff();

    // UIのガイドメニューオンにしたときの初期化
    void UIGuidMenuOn();

    // UIのガイドメニューオフにしたときの初期化
    void UIGuidMenuOff();

    // UIのタイトルへ戻るメニューオンにしたときの初期化
    void UIQuitTitleMenuOn();

    // UIのタイトルへ戻るメニューオフにしたときの初期化
    void UIQuitTitleMenuOff();

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

    // メインカメラのオブジェクトを借りて来る
    Object3D* mMainCameraObj;

    // UIの全体の親オブジェクト
    Object3D* mMenuParentObj;

    Object3D* mMenuPlaneObj;

    Object3D* mPlanesParentObj;

    std::vector<Object3D*> mMainMenuObjs;

    // UIのメインメニューの項目オブジェクト
    std::vector<UI3DTabItemStatus>mMenuUIObj;

    Object3D* mTabsParentObj;

    Object3D* mTabsPParentObj;

    Object3D* mTabBoardParentObj;

    // UIのタブボードのオブジェクト
    std::vector<Object3D*> mTabBoardObjs;

    // UIのタブメニュー項目オブジェクト
    std::vector<UI3DTabItemStatus>mMenuTabUIObj;

    std::vector<Object3D*>mMenuTabTextObj;

    // UIタブ項目内のオブジェクト
    std::vector<IUITab*> mTabItems;

    CameraTab* mCameraItem;
    GraphicsTab* mGpraphicsItem;
    SoundTab* mSoundItem;

    // UIガイド関連オブジェ
    Object3D* mGuidParentObj;
    Object3D* mGuideKeyTexObj;
    Object3D* mGuidePadTexObj;

    // UIタイトルへ戻る関連オブジェ
    Object3D* mQuitTitleParentObj;
    std::vector<UI3DTabItemStatus>mQuitTextObjs;
    Vec3 mQuitTitleSelectScale;
    Vec3 mQuitTitleDisabledScale;

    // タイトルへ戻るかどうかのフラグ
    bool IsBackToTitle;

    // UIのメニュー項目にアクセスしたときの演出
    bool IsActiveOption;

    // UIのタブのイージング
    Easing mUITabEase;
    Easing mUITabBoardEase;
    Easing mUITabAlphaEase;

    // UIイージングパラメータ
    float mTabRotaAfter;
    float mTabRotaBefore;

    float mTabBoardRotaAfter;
    float mTabBoardRotaBefore;

    float mTabEaseTimeLimit;

    float mBackTitleLimit;

    // UIのタブのオンフラグ
    bool IsUITabOn;

    // UIのガイドメニューオンフラグ
    bool IsGuidOn;

    // UIのタイトルへ戻るオンフラグ
    bool IsQuitTitleOn;

    // 現在のタブのメニュー番号
    int mCurrentTabNum;

    // プレイヤーコントロール(取得用)
    PlayerControl* mPlayerControl;

    // UIの最初に現れるときのパラメータ
    Easing mMainAlphaEase;
    Color mBackBlackAlpha;
    Color mMainMenuColor;
    Color mMainMenuTextColor;

    // UIタブ切り替え時の演出フラグ
    bool mIsTabChange;
    bool mIsTabSet;
    bool mIsTabRight;
    bool mIsTabLeft;
    float mTabRotaFirst;
    float mTabBoardParentRotaAfter;
    Easing mTabChangeEase;

    Timer mQuitTimer;

    bool mIsBackTitleMoment;
    bool mIsBackTitleOnOnOn;

    Object3D* mGuideParent;
    Object3D* mBackParent;

    Easing mGuideEase;
    Easing mBackTitleEase;

    Easing mJoJoEase;
    bool mIsJoJo;

    SpriteObject* mCursorImage;
};
RegisterScript(SuperUI);

