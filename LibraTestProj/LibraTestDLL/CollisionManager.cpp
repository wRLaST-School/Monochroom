#include "CollisionManager.h"
#include <SceneManager.h>
#include <ConsoleWindow.h>
#include <GameManager.h>
#include <PlayerControl.h>

void CollisionManager::Init()
{
	auto camera = SceneManager::FindObject<Object3D>("Camera");
	mViewCollider = SceneManager::FindChildObject<ViewCollider>("ViewCollider", camera);

	auto player = SceneManager::FindObject<Object3D>("Player");
	mPlayerCollider = SceneManager::FindChildObject<PlayerCollider>("PlayerCollider", player);

	mBlockColliders.clear();
	auto objs = SceneManager::FindObjectsWithTag<Object3D>("Block");
	for (const auto& obj : objs)
	{
		auto collider = SceneManager::FindChildObject<BlockCollider>("BlockCollider", obj);
		mBlockColliders.push_back(collider);
	}

	mFlyBlockColliders.clear();
	objs.clear();
	objs = SceneManager::FindObjectsWithTag<Object3D>("FlyBlock");
	for (const auto& obj : objs)
	{
		auto collider = SceneManager::FindChildObject<FlyBlockCollider>("FlyBlockCollider", obj);
		mFlyBlockColliders.push_back(collider);
	}
}

void CollisionManager::Update()
{
	if (!ConsoleWindow::NullCheck<PlayerCollider>(mPlayerCollider))
	{
		ConsoleWindow::Log("mPlayerCollider is Null");
		return;
	}

	// レイと飛んでくるブロック
	RayHitFlyBlocks();

	// プレイヤーとブロック
	PlayerHitBlocks();
}

void CollisionManager::Draw()
{

}

void CollisionManager::RayHitFlyBlocks()
{
	if (!mViewCollider)
	{
		return;
	}

	ConsoleWindow::Log(std::format("mFlyBlockColliders Size : {}", mFlyBlockColliders.size()));

	auto rayCollider = mViewCollider->GetRayCollider();
	for (const auto& fbc : mFlyBlockColliders)
	{
		// レイ
		auto fbBodyCollider = fbc->GetBodyCollider();
		if (rayCollider.IsTriggerSphere(&fbBodyCollider))
		{
			ConsoleWindow::Log("Ray Hit FlyBlock");
		}
	}
}

void CollisionManager::PlayerHitBlocks()
{
	auto playerBodyCollider = mPlayerCollider->GetBodyCollider();
	auto playerDownCollider = mPlayerCollider->GetDownCollider();

	for (const auto& bc : mBlockColliders)
	{
		// 押し出し
		Vec3 pushOut = Vec3::zero;
		if (bc->GetBodyCollider().IsTriggerSphere(&playerBodyCollider, &pushOut))
		{
			//SceneManager::FindObject<Object3D>("Player")->position += pushOut;
			mPlayerCollider->Parent()->CastTo<Object3D>()->position += pushOut;
		}

		// 重力
		if (bc->GetBodyCollider().IsTriggerOBB(&playerDownCollider))
		{
			auto player = mPlayerCollider->Parent()->CastTo<Object3D>();

			float posY = bc->GetBodyCollider().pos.y;
			float offsetY = bc->GetBodyCollider().scale.y + player->scale.y;
			player->position.y = posY + offsetY;

			auto playerControl = SceneManager::FindChildObject<PlayerControl>("PlayerControl", player);
			playerControl->GravityToZero();
		}
	}
}

RegisterScriptBody(CollisionManager);
