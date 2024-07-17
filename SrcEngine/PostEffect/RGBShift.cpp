#include "RGBShift.h"
#include <GPipeline.h>
#include <random>
#include <ConsoleWindow.h>

SpConstBuffer<RGBShiftCB> RGBShift::cb(true);
float RGBShift::timeAccel = 0;
float RGBShift::startTimer = 0;
float RGBShift::maxTimer = 0;

float Random(float min, float max)
{
	std::random_device rd;  // ハードウェア乱数生成器を初期化
	std::mt19937 gen(rd()); // メルセンヌ・ツイスタ乱数生成器を初期化
	std::uniform_real_distribution<float> dis(min, max); // minからmaxの範囲で一様分布を設定

	return dis(gen); // 乱数を生成して返す
}

void RGBShift::Init()
{
	std::string name = "RGBShift";
	RegisterRS(name);
	RegisterPipeline(name);

	cb.Create();

	cb.contents->shiftR = Vec2(0.006f, 0.0f);
	cb.contents->shiftB = Vec2(-0.008f, 0.0f);

	timeAccel = Random(0.01f, 0.1f);
	startTimer = Random(0, 5);
	maxTimer = std::floorf(Random(10, 100));
}

void RGBShift::Effect(const TextureKey& baseTex, const TextureKey& targetTex)
{
	cb.contents->timer += timeAccel;
	if (cb.contents->timer >= maxTimer)
	{
		timeAccel = Random(0.01f, 0.1f);
		startTimer = Random(0.0f, 0.9f);
		maxTimer = std::floorf(Random(1, 10));
		cb.contents->timer = startTimer;
	}

	cb.contents->shiftR.x = Random(0.001f, 0.01f);
	cb.contents->shiftR.y = Random(-0.005f, 0.005f);
	cb.contents->shiftB.x = -Random(0.001f, 0.01f);
	cb.contents->shiftB.y = -Random(-0.005f, 0.005f);

	IPostEffector::Effect(baseTex, targetTex, "RGBShift",
		[&]()
		{
			GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(0, cb.buffer->GetGPUVirtualAddress());
		});

	ConsoleWindow::Log(std::format("cb.contents->timer : {}", cb.contents->timer).c_str());
}
