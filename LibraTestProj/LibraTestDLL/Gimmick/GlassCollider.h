#pragma once
#include "IScriptObject.h"
#include <Object3D.h>
#include <OBBCollider.h>

class GlassCollider :
	public IScriptObject
{
private:
	Object3D* mObj;
	OBBCollider mBodyCollider;

public:
	void Init();
	void Update();
	void Draw();
	void CopyComponent(IComponent* src) { src; }

	DefDel;

public:
	OBBCollider GetBodyCollider();

};

RegisterScript(GlassCollider);

