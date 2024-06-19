#pragma once
#include "IScriptObject.h"
#include <string>
#include "Bases.h"
#include <Object3D.h>

class TitleCamera : public IScriptObject
{
private:
	float time_ = 0;
	Vec2 wiggleSpeed_ = { 0.014f,0.009f };
	Vec2 wiggleLength_ = { 0.85f,0.7f };
	Vec3 targetPos_ = { 0,0,0 };
	Vec3 templatePos_ = { 0,0,0 };
	Object3D* parent_ = nullptr;


private:
	void Init();
	void Update();
	void Draw() { ; }
	void CopyComponent(IComponent* src) { ; }

public:
	void SetWiggleSpeed(const Vec2& speed) { wiggleSpeed_ = speed; }
	void SetWiggleLength(const Vec2& length) { wiggleLength_ = length; }
	void SetTargetPos(const Vec3& pos) { targetPos_ = pos; }
	void SetTemplatePos(const Vec3& pos) { templatePos_ = pos; }
};

RegisterScript(TitleCamera);