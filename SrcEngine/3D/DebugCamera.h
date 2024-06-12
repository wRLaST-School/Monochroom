#pragma once
#include <Camera.h>

class DebugCamera
{
private:
	std::unique_ptr<Camera> debugCamera;

public:
	DebugCamera();
	void Update();
};

