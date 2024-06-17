#pragma once
#include "IScriptObject.h"
#include <Bases.h>
#include <Color.h>

class TestUi :
    public IScriptObject
{
public:
    // 初期化
    void Init()override;

    // 更新処理
    void Update()override;

    // 描画処理
    void Draw()override;

    // Imgui描画
    void OnInspectorWindowDraw()override;


private:// プライベート関数
    void 
private:
    Color circleColor_ = Color::Cyan;
};

RegisterScript(TestUi);
