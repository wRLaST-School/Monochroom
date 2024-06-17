#pragma once
#include <IComponent.h>
#include "Bases.h"

#define DefDel void Del() override {delete this;}

class ScriptComponent;

class IScriptObject : public IComponent
{
public:
	virtual void Init() {};
	virtual void Update() {};
	virtual void Draw() {};
	virtual void OnInspectorWindowDraw() {};

	virtual void Del() { delete this; };

	ScriptComponent* This();

	std::string GetClassString() override;

	ScriptComponent* body;
};