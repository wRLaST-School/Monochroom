#include "SceneManager.h"
#include "TestScene.h"
#include "Input.h"
#include "FrameRate.h"
#include <SingleCamTestScene.h>
#include <IPostEffector.h>
#include <Bloom.h>
#include <GlobalTimer.h>
#include <Transition.h>
#include <BTEditorScene.h>
#include <GameManager.h>
#include <SceneRW.h>
#include <SceneFromFile.h>
#include <ScriptComponent.h>

void SceneManager::Init()
{
	debugCamera = std::make_unique<DebugCamera>();
	Transition::Load();
	InstantTransition<TestScene>();
}

void SceneManager::Update()
{
	GlobalTimer::Update();
	ConfirmTransition();
	FrameRate::FrameStartWithWait();
	UpdateLoadState();
	Transition::Update();

	//デバッグ用シーン変更
	if (Input::Key::Down(DIK_LSHIFT) || Input::Key::Down(DIK_RSHIFT))
	{
		if (Input::Key::Triggered(DIK_D))
		{
			GameManager::sShowDebug = !GameManager::sShowDebug;
		}
	}

	if (!GameManager::sDebugTimeStop)
	{
		IComponent::UpdateAllChildComponents(currentScene.get());

		IComponent::LateUpdateAllChildComponents(currentScene.get());
	}
	else
	{
		debugCamera->Update();
	}
}

DLLExport IComponent* SceneManager::FindObjectExport(const std::string& name)
{
	IComponent* found = FindObjectRecursive(name, currentScene.get());
	if (!found)
	{
		OutputDebugStringA("Object not Found");
	}
	return found;
}

DLLExport IComponent* SceneManager::FindObjectRecursive(const std::string& name, IComponent* component)
{
	for (const auto& comp : component->GetAllComponents())
	{
		if (comp->GetName() == name)
		{
			return comp.get();
		}

		IComponent* found = FindObjectRecursive(name, comp.get());
		if (found)
		{
			return found;
		}
	}

	return nullptr;
}

DLLExport IComponent* SceneManager::FindObjectTagExport(const std::string& tag)
{
	IComponent* found = FindObjectTagRecursive(tag, currentScene.get());
	if (!found)
	{
		OutputDebugStringA("Object not Found");
	}
	return found;
}

DLLExport IComponent* SceneManager::FindObjectTagRecursive(const std::string& tag, IComponent* component)
{
	for (const auto& comp : component->GetAllComponents())
	{
		if (comp->FindTag(tag))
		{
			return comp.get();
		}

		IComponent* found = FindObjectTagRecursive(tag, comp.get());
		if (found)
		{
			return found;
		}
	}

	return nullptr;
}

DLLExport void SceneManager::FindObjectsTagExport(const std::string& tag, eastl::list<IComponent*>* pList)
{
	FindObjectsTagRecursive(tag, currentScene.get(), pList);
}

DLLExport void SceneManager::FindObjectsTagRecursive(const std::string& tag, IComponent* component, eastl::list<IComponent*>* pList)
{
	for (const auto& comp : component->GetAllComponents())
	{
		if (comp->FindTag(tag))
		{
			pList->emplace_back(comp.get());
		}

		FindObjectsTagRecursive(tag, comp.get(), pList);
	}
}

void SceneManager::Draw3D()
{
	currentScene->Draw3D();
	IComponent::DrawAllChildComponents(currentScene.get());
}

void SceneManager::DrawSprite()
{
	currentScene->DrawSprite();
	Transition::Draw();
}

void SceneManager::DrawBack()
{
	currentScene->DrawBack();
}

void SceneManager::Transition()
{
	if (loadState == LoadState::Loaded)
	{
		transitionQueued = true;
	}
}

void SceneManager::WaitForLoadAndTransition()
{
	while (loadState == LoadState::Loading)
	{
		UpdateLoadState();
		Sleep(16);
	}

	transitionQueued = true;
}

void SceneManager::ConfirmTransition()
{
	if (transitionQueued)
	{
		currentScene.release();
		currentScene = nullptr;
		currentScene.swap(nextScene);

		currentScene->Init();

		loadState = LoadState::NotInProgress;

		SpTextureManager::ReleasePerSceneTexture();
		ModelManager::ReleasePerSceneModel();
		SoundManager::ReleasePerSceneSounds();
		SpEffekseer::ReleasePerSceneEffects();

		Light::ClearAllPointLights();

		transitionQueued = false;
	}
}

void SceneManager::ReleaseScene()
{
	currentScene.reset();
	nextScene.reset();
}

IScene* SceneManager::GetCurrentScene()
{
	return currentScene.get();
}

std::unique_ptr<IScene>* SceneManager::GetNextScenePP()
{
	return &nextScene;
}

SceneManager::LoadState SceneManager::GetLoadState()
{
	return loadState;
}

IScene* SceneManager::GetScene()
{
	return currentScene.get();
}

template <class NextScene, class... Args> void SceneManager::InstantTransition(Args... args)
{
	currentScene.release();
	currentScene = nullptr;
	Light::Init();
	currentScene = std::make_unique<NextScene>(args...);
	currentScene->LoadResources();
	currentScene->Init();
	FrameRate::InitMark();
}

void SceneManager::UpdateLoadState()
{
	if (loadState == LoadState::Loading && loadFinished)
	{
		loadState = LoadState::Loaded;
		loadFinished = false;
	}
}

eastl::list<IComponent*> SceneManager::FindScripts()
{
	eastl::list<IComponent*> foundList;

	FindScriptsRecursive(GetCurrentScene(), &foundList);

	return foundList;
}

void SceneManager::FindScriptsRecursive(IComponent* component, eastl::list<IComponent*>* pList)
{
	for (const auto& comp : component->GetAllComponents())
	{
		if (comp->CastTo<ScriptComponent>())
		{
			pList->emplace_back(comp.get());
		}

		FindScriptsRecursive(comp.get(), pList);
	}
}
