#pragma once
#include "IScriptObject.h"
#include <Object3D.h>
#include <RayCollider.h>

class ViewCollider :
	public IScriptObject
{
private:
	Object3D* mObj;
	RayCollider mRayCollider;

public:
	void Init();
	void Update();
	void Draw();
	void CopyComponent(IComponent* src) { src; }

	DefDel;

public:
	RayCollider GetRayCollider();

	//const 参照だとエラー出るので
	Vec3 GetPos();

};

RegisterScript(ViewCollider);

