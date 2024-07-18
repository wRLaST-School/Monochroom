#pragma once
#include <IPostEffector.h>

struct StageGeenratingInfo
{
	bool isDraw = false;
};

class StageGenerating : public IPostEffector
{
public:
	DLLExport static StageGeenratingInfo info;

public:
	static void CreateRS();
	static void Init();
	static void Effect(const TextureKey& baseTex, const TextureKey& targetTex);
};

