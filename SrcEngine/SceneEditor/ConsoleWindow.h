#pragma once
#include <string>
#include <vector>

class ConsoleWindow
{
private:
	bool mIsOpen;
	std::vector<std::string> logs;

public:
	ConsoleWindow();
	void DrawWindow();
	static void SDraw();

public:
	static DLLExport void Log(const std::string& log);

	template<typename T>
	inline static bool NullCheck(T* instance)
	{
		if (!instance)
		{
			GetInstance()->logs.push_back("NullCheck : Null");
			return false;
		}
		return true;
	}

public:
	// セッター
	void SetisOpen(bool isOpen);

public:
	// ゲッター
	bool GetisOpen();
	static DLLExport ConsoleWindow* GetInstance();

private:
	// singleton
	~ConsoleWindow() = default;
	ConsoleWindow(const ConsoleWindow&) = delete;
	ConsoleWindow(ConsoleWindow&&) = delete;
	ConsoleWindow& operator=(const ConsoleWindow&) = delete;
	ConsoleWindow& operator=(ConsoleWindow&&) = delete;

};

