#pragma once
#include "IScriptObject.h"
#include <Object3D.h>
#include <Vec2.h>
#include <Vec3.h>

class SelectPanel :
    public IScriptObject
{
public:
	void Init();
	void Update();
	void Draw();
	void OnInspectorWindowDraw();

    DefDel;

private:

	// ステージセレクト時に選択用のオブジェクト
	std::vector<std::vector<Object3D*>> mStageNumObj;

	// ステージセレクト二次元配列 X Y 番号
	Vec2 mSelectMaxNum;
	Vec2 mSelectCurrentNum;


};
RegisterScript(SelectPanel);
