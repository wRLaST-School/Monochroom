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
	static void Log(const std::string& log);

public:
	// セッター
	void SetisOpen(bool isOpen);

public:
	// ゲッター
	bool GetisOpen();
	static ConsoleWindow* GetInstance();

private:
	// singleton
	~ConsoleWindow() = default;
	ConsoleWindow(const ConsoleWindow&) = delete;
	ConsoleWindow(ConsoleWindow&&) = delete;
	ConsoleWindow& operator=(const ConsoleWindow&) = delete;
	ConsoleWindow& operator=(ConsoleWindow&&) = delete;

};

