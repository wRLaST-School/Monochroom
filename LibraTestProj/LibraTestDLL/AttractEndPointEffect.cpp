#include "AttractEndPointEffect.h"
#include <LineDrawer.h>

void AttractEndPointEffect::Init()
{
}

void AttractEndPointEffect::Update()
{
}

void AttractEndPointEffect::Draw()
{
	LineDrawer::DrawLine(mPosition);
}
