#pragma once

class GameManager {
public:
	static bool sShowDebug;
	static bool sShowHitBoxes;
#ifdef _DEBUG
	inline static bool sDebugTimeStop = true;
#else
	inline static bool sDebugTimeStop = false;
#endif
};
