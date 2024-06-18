#pragma once
#include <Camera.h>

class DebugCamera
{
private:
	std::unique_ptr<Camera> debugCamera;
	float yawAngle;
	float pitchAngle;

public:
	DebugCamera();
	void Update();
};

