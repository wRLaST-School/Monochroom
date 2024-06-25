#include "CollisionManager.h"
#include <ConsoleWindow.h>
#include <GameManager.h>
#include <PlayerControl.h>

void CollisionManager::Init()
{
	auto camera = SceneManager::FindObject<Object3D>("Camera");
	mViewCollider = SceneManager::FindChildObject<ViewCollider>("ViewCollider", camera);

	auto player = SceneManager::FindObject<Object3D>("Player");
	mPlayerCollider = SceneManager::FindChildObject<PlayerCollider>("PlayerCollider", player);

	mBlockColliders = FindColliderList<BlockCollider>("Block", "BlockCollider");
	mFlyBlockColliders = FindColliderList<FlyBlockCollider>("FlyBlock", "FlyBlockCollider");
	mButtonColliders = FindColliderList<ButtonCollider>("Button", "ButtonCollider");
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

	// プレイヤーとボタン
	PlayerHitButtons();
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
		if (rayCollider.IsTrigger(&fbBodyCollider))
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
		if (bc->GetBodyCollider().IsTrigger(&playerBodyCollider, &pushOut))
		{
			//SceneManager::FindObject<Object3D>("Player")->position += pushOut;
			mPlayerCollider->Parent()->CastTo<Object3D>()->position += pushOut;
		}

		// 重力
		if (bc->GetBodyCollider().IsTrigger(&playerDownCollider))
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

void CollisionManager::PlayerHitButtons()
{
	auto playerDownCollider = mPlayerCollider->GetDownCollider();

	for (const auto& bc : mButtonColliders)
	{
		// 重力
		if (bc->GetBodyCollider().IsTrigger(&playerDownCollider))
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
