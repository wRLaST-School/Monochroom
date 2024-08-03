#include "stdafx.h"
#include "GameManager.h"

#ifdef _DEBUG
bool GameManager::sShowDebug = true;
#else
bool GameManager::sShowDebug = false;
#endif
bool GameManager::sShowHitBoxes = false;