#include "TestUi.h"
#include <SpDS.h>
#include <SpImGui.h>
#include <SpTextureManager.h>

void TestUi::Init()
{
	std::vector<TextureKey> keys = {
		"circle1",
		"circle2",
		"circle3",
		"circle4"
	};

	SpTextureManager::LoadDiv("Assets/Images/circleParticle.png", 256, 256, 2, 2, keys);
}

void TestUi::Update()
{
}

void TestUi::Draw()
{


	SpDS::DrawRotaGraph(200, 400, 1, 1, 0, "circle1", Anchor::Center, Color(255, 120, 120, 255));
	
}

void TestUi::OnInspectorWindowDraw()
{
	/*ImGui::ColorEdit4("circleColor", &circleColor_.f4.x);*/
}
