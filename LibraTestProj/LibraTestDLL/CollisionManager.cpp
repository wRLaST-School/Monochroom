#include "CollisionManager.h"
#include <ConsoleWindow.h>
#include <GameManager.h>
#include <PlayerControl.h>
#include <FlyBlock.h>
#include <Input.h>
#include <AppOperationCommand.h>

void CollisionManager::Init()
{
	auto camera = SceneManager::FindObject<Object3D>("Camera");
	mViewCollider = SceneManager::FindChildObject<ViewCollider>("ViewCollider", camera);

	auto player = SceneManager::FindObject<Object3D>("Player");
	mPlayerCollider = SceneManager::FindChildObject<PlayerCollider>("PlayerCollider", player);

	mBlockColliders = FindColliderList<BlockCollider>("Block", "BlockCollider");
	mFlyBlockColliders = FindColliderList<FlyBlockCollider>("FlyBlock", "FlyBlockCollider");
	mButtonColliders = FindColliderList<ButtonCollider>("Button", "ButtonCollider");
	mGlassColliders = FindColliderList<GlassCollider>("Glass", "GlassCollider");
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

	// プレイヤーとガラス
	PlayerHitGlasses();

	// 飛んでくるブロックとブロック
	FlyBlocksHitBlocks();
}

void CollisionManager::RayHitFlyBlocks()
{
	if (!mViewCollider)
	{
		return;
	}

	auto rayCollider = mViewCollider->GetRayCollider();
	for (const auto& fbc : mFlyBlockColliders)
	{
		// レイ
		auto fbBodyCollider = fbc->GetBodyCollider();
		if (rayCollider.IsTrigger(&fbBodyCollider))
		{
			if (AppOperationCommand::GetInstance()->PlayerAttractBlockCommand())
			{
				//引き寄せる
				auto flyblock = fbc->Parent()->GetComponent("FlyBlock");
				flyblock->Activate();
				//auto flyBlock2 = flyblock->CastTo<FlyBlock>();
				
				/*if (a == nullptr)
				{
				}
				else
				{
					flyblock->Activate();
				}*/

				//flyBlock2->Activate();
				//flyBlock2->BeginAttracting(mViewCollider->GetPos());
			}

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
			mPlayerCollider->Parent()->CastTo<Object3D>()->position += pushOut;
		}

		// 重力
		if (bc->GetBodyCollider().IsTrigger(&playerDownCollider))
		{
			auto player = mPlayerCollider->Parent()->CastTo<Object3D>();
			auto playerControl = SceneManager::FindChildObject<PlayerControl>("PlayerControl", player);

			if (playerControl->GetGravity()->GetVelocity().y <= 0.f)
			{
				float posY = bc->GetBodyCollider().pos.y;
				float offsetY = bc->GetBodyCollider().scale.y + player->scale.y * 2;
				player->position.y = posY + offsetY;

				playerControl->GravityToZero();
			}
		}
	}
}

void CollisionManager::PlayerHitButtons()
{
	auto playerDownCollider = mPlayerCollider->GetDownCollider();
	ConsoleWindow::Log(std::format("ButtonColliders Size : {}", mButtonColliders.size()));

	for (const auto& bc : mButtonColliders)
	{
		// 重力
		if (bc->GetBodyCollider().IsTrigger(&playerDownCollider))
		{
			auto player = mPlayerCollider->Parent()->CastTo<Object3D>();

			float posY = bc->GetBodyCollider().pos.y;
			float offsetY = bc->GetBodyCollider().scale.y + player->scale.y * 2;
			player->position.y = posY + offsetY;

			auto playerControl = SceneManager::FindChildObject<PlayerControl>("PlayerControl", player);
			playerControl->GravityToZero();
		}
		else if (bc->GetFrameCollider().IsTrigger(&playerDownCollider))
		{
			auto player = mPlayerCollider->Parent()->CastTo<Object3D>();

			float posY = bc->GetFrameCollider().pos.y;
			float offsetY = bc->GetFrameCollider().scale.y + player->scale.y * 2;
			player->position.y = posY + offsetY;

			auto playerControl = SceneManager::FindChildObject<PlayerControl>("PlayerControl", player);
			playerControl->GravityToZero();
		}
	}
}

void CollisionManager::PlayerHitGlasses()
{
	auto playerBodyCollider = mPlayerCollider->GetBodyCollider();

	for (const auto& gc : mGlassColliders)
	{
		// 押し出し
		Vec3 pushOut = Vec3::zero;
		if (gc->GetBodyCollider().IsTrigger(&playerBodyCollider, &pushOut))
		{
			mPlayerCollider->Parent()->CastTo<Object3D>()->position += pushOut;
		}
	}
}

void CollisionManager::FlyBlocksHitBlocks()
{
	for (const auto& fbc : mFlyBlockColliders)
	{
		auto flyBlockBodyCollider = fbc->GetBodyCollider();
		auto flyBlockDownCollider = fbc->GetDownCollider();

		for (const auto& bc : mBlockColliders)
		{
			// 押し出し
			Vec3 pushOut = Vec3::zero;
			if (bc->GetBodyCollider().IsTrigger(&flyBlockBodyCollider, &pushOut))
			{
				fbc->Parent()->CastTo<Object3D>()->position += pushOut;
			}

			// 重力
			if (bc->GetBodyCollider().IsTrigger(&flyBlockDownCollider))
			{
				auto flyBlock = fbc->Parent()->CastTo<Object3D>();

				float posY = bc->GetBodyCollider().pos.y;
				float offsetY = bc->GetBodyCollider().scale.y + flyBlock->scale.y;
				flyBlock->position.y = posY + offsetY;
			}
		}
	}
}

void CollisionManager::FlyBlocksHitButtons()
{

}

void CollisionManager::FlyBlocksHitGlasses()
{

}

RegisterScriptBody(CollisionManager);
