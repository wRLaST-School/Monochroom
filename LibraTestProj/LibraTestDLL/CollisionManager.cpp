#include "CollisionManager.h"
#include <ConsoleWindow.h>
#include <GameManager.h>
#include <PlayerControl.h>
#include <FlyBlock.h>
#include <Input.h>
#include <AppOperationCommand.h>
#include <Camera.h>

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

	//カメラにブロックが映っているか
	CameraInsideFlyBlocks();

	// 飛んでくるブロックとボタン
	FlyBlocksHitButtons();

	// 飛んでくるブロックとガラス
	FlyBlocksHitGlasses();

	// 飛んでくるブロックと飛んでくるブロック
	FlyBlocksHitFlyBlocks();
}

void CollisionManager::RayHitFlyBlocks()
{
	if (!mViewCollider)
	{
		return;
	}

	if (AppOperationCommand::GetInstance()->PlayerAttractBlockCommand())
	{
		FlyBlock* flyBlock = nullptr;
		auto rayCollider = mViewCollider->GetRayCollider();
		float minDis = 99999.f;

		for (const auto& fbc : mFlyBlockColliders)
		{
			// レイ
			auto fbBodyCollider = fbc->GetBodyCollider();
			if (rayCollider.IsTrigger(&fbBodyCollider))
			{
				float dis = Vec3::Distance(fbc->Parent()->CastTo<Object3D>()->position, rayCollider.r.origin);
				if (dis < minDis)
				{
					flyBlock = SceneManager::FindChildObject<FlyBlock>("FlyBlock", fbc->Parent());
				}
			}
		}

		// 一番近いFlyBlockのみ引き寄せる
		if (flyBlock)
		{
			flyBlock->BeginAttracting(mViewCollider->GetPos() + Vec3{ 0,2.0f,0 });
		}
	}
}

void CollisionManager::CameraInsideFlyBlocks()
{
	for (const auto& fbc : mFlyBlockColliders)
	{
		auto flyblock = SceneManager::FindChildObject<FlyBlock>("FlyBlock", fbc->Parent());

		if (flyblock->GetIsAttracting())
		{
			auto camera = SceneManager::FindObjectWithTag<Camera>("PlayerCamera");

			//画面外に出たら落ちる
			if (!camera->CheckisInCameraInside(fbc->Parent()->CastTo<Object3D>()->position))
			{
				flyblock->EndAttracting();
			}
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
		else if (bc->GetFlameCollider().IsTrigger(&playerDownCollider))
		{
			auto player = mPlayerCollider->Parent()->CastTo<Object3D>();

			float posY = bc->GetFlameCollider().pos.y;
			float offsetY = bc->GetFlameCollider().scale.y + player->scale.y * 2;
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
			auto flyblock = SceneManager::FindChildObject<FlyBlock>("FlyBlock", fbc->Parent());

			// 押し出し
			Vec3 pushOut = Vec3::zero;
			if (bc->GetBodyCollider().IsTrigger(&flyBlockBodyCollider, &pushOut))
			{
				fbc->Parent()->CastTo<Object3D>()->position += pushOut;

				if (flyblock->GetAttractedDir().Dot(-pushOut) > FlyBlock::skAttractedHittingNotEndDot)
				{
					flyblock->EndAttracting();
				}
			}

			// 重力
			if (bc->GetBodyCollider().IsTrigger(&flyBlockDownCollider) &&
				!flyblock->GetIsAttracting())
			{
				auto flyBlockObj3D = fbc->Parent()->CastTo<Object3D>();

				float posY = bc->GetBodyCollider().pos.y;
				float offsetY = bc->GetBodyCollider().scale.y + flyBlockObj3D->scale.y;
				flyBlockObj3D->position.y = posY + offsetY;

				flyblock->ZeroGravity();
				flyblock->EndAttracting();
			}
		}
	}
}

void CollisionManager::FlyBlocksHitButtons()
{
	for (const auto& fbc : mFlyBlockColliders)
	{
		auto flyBlockDownCollider = fbc->GetDownCollider();
		for (const auto& bc : mButtonColliders)
		{
			// 重力
			auto buttonBodyCollider = bc->GetBodyCollider();
			auto buttonFlameCollider = bc->GetFlameCollider();
			if (flyBlockDownCollider.IsTrigger(&buttonBodyCollider))
			{
				// 飛ぶブロック
				auto flyBlock = SceneManager::FindChildObject<FlyBlock>("FlyBlock", fbc->Parent());
				if (flyBlock->GetGravity()->GetVelocity().y <= 0.f)
				{
					float posY = buttonBodyCollider.pos.y;
					float offsetY = (flyBlockDownCollider.scale.y * 2) + buttonBodyCollider.scale.y;

					fbc->Parent()->CastTo<Object3D>()->position.y = posY + offsetY;
					flyBlock->ZeroGravity();
				}

				// ボタン
				///
				/// 押す状態にする処理をここに
				/// 

			}
			else if (flyBlockDownCollider.IsTrigger(&buttonFlameCollider))
			{
				// 飛ぶブロック
				auto flyBlock = SceneManager::FindChildObject<FlyBlock>("FlyBlock", fbc->Parent());
				if (flyBlock->GetGravity()->GetVelocity().y <= 0.f)
				{
					float posY = buttonFlameCollider.pos.y;
					float offsetY = (flyBlockDownCollider.scale.y * 2) + buttonFlameCollider.scale.y;

					fbc->Parent()->CastTo<Object3D>()->position.y = posY + offsetY;
					flyBlock->ZeroGravity();
				}

			}
		}
	}
}

void CollisionManager::FlyBlocksHitGlasses()
{
	for (const auto& fbc : mFlyBlockColliders)
	{
		auto flyBlockMoveCollider = fbc->GetMoveCollider();

		for (const auto& gc : mGlassColliders)
		{
			auto flyblock = SceneManager::FindChildObject<FlyBlock>("FlyBlock", fbc->Parent());

			// 押し出し
			Vec3 pushOut = Vec3::zero;
			if (gc->GetBodyCollider().IsTrigger(&flyBlockMoveCollider, &pushOut))
			{
				fbc->Parent()->CastTo<Object3D>()->position += pushOut;
				flyblock->EndAttracting();
			}
		}
	}
}

void CollisionManager::FlyBlocksHitFlyBlocks()
{
	for (const auto& fbc1 : mFlyBlockColliders)
	{
		for (const auto& fbc2 : mFlyBlockColliders)
		{
			if (fbc1 == fbc2)
			{
				continue;
			}

			// ブロック1
			auto flyBlockBodyCollider1 = fbc1->GetBodyCollider();
			auto flyBlockMoveCollider1 = fbc1->GetMoveCollider();
			auto flyBlockDownCollider1 = fbc1->GetDownCollider();

			// ブロック2
			auto flyBlockBodyCollider2 = fbc2->GetBodyCollider();
			auto flyBlockMoveCollider2 = fbc2->GetMoveCollider();
			auto flyBlockDownCollider2 = fbc2->GetDownCollider();

			// 重力
			if (flyBlockDownCollider1.IsTrigger(&flyBlockBodyCollider2))
			{
				auto flyBlock1 = SceneManager::FindChildObject<FlyBlock>("FlyBlock", fbc1->Parent());
				if (flyBlock1->GetGravity()->GetVelocity().y <= 0.f)
				{
					float posY = flyBlockBodyCollider2.pos.y;
					float offsetY = (flyBlockDownCollider1.scale.y * 2) + flyBlockBodyCollider2.scale.y;

					fbc1->Parent()->CastTo<Object3D>()->position.y = posY + offsetY;

					flyBlock1->ZeroGravity();
				}
			}

			// 押し戻し
			Vec3 pushOut = Vec3::zero;
			if (flyBlockBodyCollider1.IsTrigger(&flyBlockBodyCollider2, &pushOut))
			{
				FlyBlock* flyBlock = SceneManager::FindChildObject<FlyBlock>("FlyBlock", fbc1->Parent());
				if (flyBlock)
				{
					if (flyBlock->GetMoveVec().GetSquaredLength() != 0)
					{
						fbc1->Parent()->CastTo<Object3D>()->position += pushOut;
						flyBlock->EndAttracting();
					}
				}
			}
		}
	}
}

RegisterScriptBody(CollisionManager);
