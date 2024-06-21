#pragma once
class TagWindow
{
public:
	inline static bool open = false;
	inline static bool prevOpen = false;
	inline static eastl::list<std::string> tags;

	inline static std::string savePath = "Resources/tags.spop";

	static void Show();

	static void Hide();

	static void Draw();

	static void DDSource();

	static void SaveTags();
	static void LoadTags();

	static void OnOpenWindow();
	static void OnCloseWindow();
private:
	// singleton
	~TagWindow() = default;
	TagWindow(const TagWindow&) = delete;
	TagWindow(TagWindow&&) = delete;
	TagWindow& operator=(const TagWindow&) = delete;
	TagWindow& operator=(TagWindow&&) = delete;
};

