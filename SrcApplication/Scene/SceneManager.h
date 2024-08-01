#pragma once
#include "IScene.h"
#include "Essentials.h"
#include <future>
#include <thread>
#include <SoundManager.h>
#include <SpEffekseer.h>
#include <DebugCamera.h>

class SceneManager final
{
public:
	static DLLExport void Init();
	static DLLExport void Update();
	static DLLExport void Draw3D();
	static DLLExport void DrawSprite();
	static DLLExport void DrawBack();

	//非同期で次のシーンの読み込みを開始する
	template <class NextScene, class... Args> static void LoadScene(Args... args)
	{
		if (GetLoadState() != LoadState::NotInProgress)
		{
			return;
		}

		*GetNextScenePP() = std::make_unique<NextScene>(args...);

		SetLoadState(LoadState::Loading);

		ftr = std::async(std::launch::async, [&] {
			SpTextureManager::PreLoadNewScene();
			ModelManager::PreLoadNewScene();
			SoundManager::PreLoadNewScene();
			SpEffekseer::PreLoadNewScene();
			(*GetNextScenePP())->LoadResources();
			SetLoadFinished();
			});
	};

	//読み込みが終わっていたらシーンを切り替え、終わっていないなら何もしない
	static DLLExport void Transition();

	//読み込みが終わるまで処理を止めて待機してからシーンを切り替え
	static DLLExport void WaitForLoadAndTransition();

	static DLLExport void ConfirmTransition();

	static DLLExport void ReleaseScene();

	static DLLExport IScene* GetCurrentScene();
	static DLLExport std::unique_ptr<IScene>* GetNextScenePP();

	enum class LoadState {
		NotInProgress,
		Loading,
		Loaded
	};
	//現在のシーン読み込みの状態を取得
	static DLLExport LoadState GetLoadState();
	static DLLExport void SetLoadState(LoadState loadState);

	static DLLExport IScene* GetScene();

	inline static std::unique_ptr<IScene> currentScene;
	inline static std::unique_ptr<IScene> nextScene;
#pragma region オブジェクト検索関連
private:

	static DLLExport IComponent* FindObjectExport(const std::string& name);
	static DLLExport IComponent* FindObjectRecursive(const std::string& name, IComponent* component);

	static DLLExport IComponent* FindObjectTagExport(const std::string& tag);
	static DLLExport IComponent* FindObjectTagRecursive(const std::string& tag, IComponent* component);

	static DLLExport void FindObjectsTagExport(const std::string& tag, eastl::list<IComponent*>* pList);
	static DLLExport void FindObjectsTagRecursive(const std::string& tag, IComponent* component, eastl::list<IComponent*>* pList);

public:
	template<typename T>
	inline static T* FindObject(const std::string& name)
	{
		return FindObjectExport(name)->CastToScript<T>();
	}

	template<class T>
	inline static T* FindChildObject(const std::string& name, IComponent* root) {
		return FindObjectRecursive(name, root)->CastToScript<T>();
	}

	template<class T>
	inline static T* FindObjectWithTag(const std::string& tag)
	{
		return FindObjectTagExport(tag)->CastToScript<T>();
	}

	template<class T>
	inline static T* FindChildObjectWithTag(const std::string& tag, IComponent* root)
	{
		return FindObjectTagRecursive(tag, root)->CastToScript<T>();
	}

	template<class T>
	inline static eastl::list<T*> FindObjectsWithTag(const std::string& tag)
	{
		eastl::list<T*> foundList;

		eastl::list<IComponent*> notCasted;

		FindObjectsTagExport(tag, &notCasted);

		for (auto& nc : notCasted)
		{
			foundList.emplace_back(nc->CastToScript<T>());
		}

		return foundList;
	}

	template<class T>
	inline static eastl::list<T*> FindChildObjectsWithTag(const std::string& tag, IComponent* root)
	{
		eastl::list<T*> foundList;

		eastl::list<IComponent*> notCasted;

		FindObjectsTagRecursive(tag, root, notCasted);

		for (auto& nc : notCasted)
		{
			foundList.emplace_back(nc->CastToScript<T>());
		}

		return foundList;
	}
#pragma endregion 

private:
	//ロードの状態、毎フレームの最初に更新される
	inline static LoadState loadState;
	//こっちはリアルタイム更新、次フレームの最初にリセット
	DLLExport static void SetLoadFinished();
	inline static bool loadFinished;
	static DLLExport void UpdateLoadState();
	inline static std::unique_ptr<DebugCamera> debugCamera;

public:
	DLLExport inline static bool transitionQueued;

	//以下基本使用禁止
public:
	//非同期でのシーン読み込みを行わずに直接シーン切り替えをする(バグ起きがちなので注意)
	template <class NextScene, class... Args> static void InstantTransition(Args... args);

	static DLLExport eastl::list<IComponent*> FindScripts();
	static DLLExport void FindScriptsRecursive(IComponent* component, eastl::list<IComponent*>* pList);
private:
	SceneManager();
	~SceneManager() {};
	SceneManager(const SceneManager& a) = delete;
	SceneManager& operator=(const SceneManager& a) = delete;
	inline static std::future<void> ftr;
};