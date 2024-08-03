#include "CollisionManager.h"
#include <ConsoleWindow.h>
#include <GameManager.h>
#include <PlayerControl.h>
#include <FlyBlock.h>
#include <Input.h>
#include <AppOperationCommand.h>
#include <Camera.h>
#include <StageButton.h>
#include <StageDoor.h>
#include <GoggleScr.h>
#include <PlayerGoggle.h>
#include <StageGoal.h>
#include <AttractParticleManager.h>
#include <BlockWhite.h>
#include <FlyBlockWhite.h>
#include <GrayScale.h>
#include <SpriteObject.h>

void CollisionManager::Init()
{
	auto camera = SceneManager::FindObject<Object3D>("Camera");
	mViewCollider = SceneManager::FindChildObject<ViewCollider>("ViewCollider", camera);

	auto player = GameManager::GetInstance()->GetPlayer();
	mPlayerCollider = SceneManager::FindChildObject<PlayerCollider>("PlayerCollider", player);
	mPlayerGoggle = SceneManager::FindChildObject<PlayerGoggle>("PlayerGoggle", player);

	mBlockColliders = FindColliderList<BlockCollider>("Block", "BlockCollider");

	mFlyBlockColliders = FindColliderList<FlyBlockCollider>("FlyBlock", "FlyBlockCollider");

	mButtonColliders = FindColliderList<ButtonCollider>("Button", "ButtonCollider");
	mGlassColliders = FindColliderList<GlassCollider>("Glass", "GlassCollider");
	mGoalColliders = FindColliderList<GoalCollider>("Goal", "GoalCollider");
	mGoggleColliders = FindColliderList<GoggleCollider>("Goggle", "GoggleCollider");
	mTransparentColliders = FindColliderList<TransparentCollider>("Transparent", "TransparentCollider");
	mDoorColliders = FindColliderList<DoorCollider>("StageDoor", "DoorCollider");

	mBlockCollEffect = SceneManager::FindObject<BlockCollEffect>("BlockEffectScript");
}

void CollisionManager::Update()
{
	if (!ConsoleWindow::NullCheck<PlayerCollider>(mPlayerCollider))
	{
		ConsoleWindow::Log("mPlayerCollider is Null");
		return;
	}

	bool isEquipGoggle = mPlayerGoggle->GetFinishedEquipGoggle();

	//カメラにブロックが映っているか
	CameraInsideFlyBlocks();

	// レイと飛んでくるブロック
	RayHitFlyBlocks();

	// プレイヤーとブロック
	PlayerHitBlocks();

	// プレイヤーとボタン
	PlayerHitButtons();

	if (!isEquipGoggle)
	{
		// プレイヤーとガラス
		PlayerHitGlasses();
	}

	// プレイヤーとゴール
	PlayerHitGoals();

	// プレイヤーと透明の壁
	PlayerHitTransparents();

	// プレイヤーとドア
	PlayerHitDoors();

	// 飛んでくるブロックとブロック
	FlyBlocksHitBlocks();

	// 飛んでくるブロックとボタン
	FlyBlocksHitButtons();

	if (!isEquipGoggle)
	{
		// 飛んでくるブロックとガラス
		FlyBlocksHitGlasses();
	}

	// 飛んでくるブロックと飛んでくるブロック
	FlyBlocksHitFlyBlocks();

	//プレイヤーとゴーグル
	RayHitGoggle();

	// 飛んでくるブロックとゴール
	FlyBlocksHitGoals();

	// 飛ぶブロックトドアの当たり判定
	FlyBlocksHitDoors();

	//白いブロックと白い壁
	CheckRayHitWhFlyBlockAndWhWall();
}

void CollisionManager::Draw()
{
	Color col = Color::Green;
	coll1.DrawCollider();
	coll1.color = col;
	coll2.DrawCollider();
	coll2.color = col;
}

void CollisionManager::CameraInsideFlyBlocks()
{
	for (const auto& fbc : mFlyBlockColliders)
	{
		auto flyblock = GameManager::GetInstance()->GetFlyBlock(fbc->Parent());

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

void CollisionManager::RayHitFlyBlocks()
{
	if (!mViewCollider)
	{
		return;
	}

	float dis = 0;
	float minDis = 0;

	//引き寄せ
	FlyBlock* flyBlock = nullptr;
	auto rayCollider = mViewCollider->GetRayCollider();

	for (const auto& fbc : mFlyBlockColliders)
	{
		minDis = CheckRayHitOtherDis(fbc);

		// レイ
		auto fbBodyCollider = fbc->GetBodyCollider();
		if (rayCollider.IsTrigger(&fbBodyCollider))
		{
			dis = rayCollider.disToInter;
			if (dis < minDis || GrayScale::GetIsGrayScale())
			{
				if (AppOperationCommand::GetInstance()->PlayerAttractBlockCommand())
				{
					flyBlock = GameManager::GetInstance()->GetFlyBlock(fbc->Parent());
				}
			}
		}

		// 一番近いFlyBlockのみ引き寄せる
		if (flyBlock)
		{
			Vec3 dirVec = mViewCollider->GetRayCollider().r.ray;

			// 仮に移動させて当たるがどうかをチェックする用
			if (CheckHitOther(fbc) && !GrayScale::GetIsGrayScale())
			{
				// 当たっていたら移動させない
				flyBlock = nullptr;
				continue;
			}

			auto ePos = mViewCollider->GetPos() + Vec3{ dirVec.x,0,dirVec.z }.GetNorm() * 2.0f + Vec3{ 0,-0.5f,0 };
			flyBlock->BeginAttracting(ePos);
			fbc->IsMoveing();

			// 再帰関数で上に乗せてるやつをチェックする
			RecursiveAttracting(fbc, mFlyBlockColliders);

			//引き寄せの演出
			//始点と終点を決めて演出開始
			auto particleManager = GameManager::GetInstance()->GetAttractParticleManager();
			if (particleManager)
			{
				particleManager->
					BeginAttractEffect(flyBlock,
						flyBlock->Parent()->CastTo<Object3D>()->position, ePos);
			}

			flyBlock = nullptr;
		}
	}
}

void CollisionManager::RayHitGoggle()
{
	if (!mViewCollider)
	{
		return;
	}

	//ゴーグル取得
	if (AppOperationCommand::GetInstance()->PlayerGetGoggleCommand())
	{
		auto player = mPlayerCollider->Parent()->CastTo<Object3D>();
		auto playerGoggle = SceneManager::FindChildObject<PlayerGoggle>("PlayerGoggle", player);

		if (playerGoggle->GetIsHavingGoggle())
		{
			return;
		}


		GoggleScr* goggleScr = nullptr;
		auto rayCollider = mViewCollider->GetRayCollider();
		float minDis = 99999.f;

		for (const auto& gc : mGoggleColliders)
		{
			ConsoleWindow::Log("GoggleIsHere");

			// レイ
			auto gscCollider = gc->GetSphereCollider();
			if (rayCollider.IsTrigger(&gscCollider))
			{
				float dis = Vec3::Distance(gc->Parent()->CastTo<Object3D>()->position, rayCollider.r.origin);
				if (dis < minDis)
				{
					goggleScr = SceneManager::FindChildObject<GoggleScr>("GoggleScr", gc->Parent());
				}
			}
		}

		// ゴーグルをプレイヤーと紐づけ(親子関係
		if (goggleScr)
		{
			GameManager::GetInstance()->PlayerGettedGoggle(player, goggleScr);
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
		//ステージ内のボタン
		auto button = bc->Parent()->CastTo<Object3D>();
		auto stageButton = SceneManager::FindChildObject<StageButton>("StageButton", button);


		// 重力
		//body
		if (bc->GetBodyCollider().IsTrigger(&playerDownCollider))
		{
			auto player = mPlayerCollider->Parent()->CastTo<Object3D>();

			float posY = bc->GetBodyCollider().pos.y;
			float offsetY = bc->GetBodyCollider().scale.y + player->scale.y * 2;
			player->position.y = posY + offsetY;

			auto playerControl = SceneManager::FindChildObject<PlayerControl>("PlayerControl", player);
			playerControl->GravityToZero();

			//ボタンへこませる
			stageButton->BeginPushingButton();

			//同じタグを持つドアがあれば開ける
			for (auto& door : SceneManager::FindObjectsWithTag<Object3D>("StageDoor"))
			{
				std::string str = button->GetSameTag(*door->CastTo<IComponent>(), "DB");

				if (str.size())
				{
					ConsoleWindow::Log(str);
					auto linkDoor = SceneManager::FindChildObject<StageDoor>("StageDoor", door);
					linkDoor->OpenDoor();
				}
			}
		}
		//frame
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
	auto playerDownCollider = mPlayerCollider->GetDownCollider();

	for (const auto& gc : mGlassColliders)
	{
		// 押し出し
		Vec3 pushOut = Vec3::zero;
		if (gc->GetBodyCollider().IsTrigger(&playerBodyCollider, &pushOut))
		{
			mPlayerCollider->Parent()->CastTo<Object3D>()->position += pushOut;
		}

		// 重力
		if (gc->GetBodyCollider().IsTrigger(&playerDownCollider))
		{
			auto player = mPlayerCollider->Parent()->CastTo<Object3D>();
			auto playerControl = SceneManager::FindChildObject<PlayerControl>("PlayerControl", player);

			if (playerControl->GetGravity()->GetVelocity().y <= 0.f)
			{
				float posY = gc->GetBodyCollider().pos.y;
				float offsetY = gc->GetBodyCollider().scale.y + player->scale.y * 2;
				player->position.y = posY + offsetY;

				playerControl->GravityToZero();
			}
		}
	}
}

void CollisionManager::PlayerHitGoals()
{
	auto playerBodyCollider = mPlayerCollider->GetBodyCollider();

	for (const auto& gc : mGoalColliders)
	{
		Vec3 diff = playerBodyCollider.pos - gc->GetClearCollider().pos;
		diff.y = 0;

		if (diff.GetLength() <= 0.5f)
		{
			auto goal = gc->Parent();
			auto stageGoal = SceneManager::FindChildObject<StageGoal>("StageGoal", goal);

			if (stageGoal->clearState == StageGoal::None)
			{
				stageGoal->clearState = StageGoal::ChangeScene;
			}
			ConsoleWindow::Log("Hit Goal");

		}
	}
}

void CollisionManager::PlayerHitTransparents()
{
	auto playerBodyCollider = mPlayerCollider->GetBodyCollider();

	for (const auto& tc : mTransparentColliders)
	{
		// 押し出し
		Vec3 pushOut = Vec3::zero;
		if (tc->GetBodyCollider().IsTrigger(&playerBodyCollider, &pushOut))
		{
			mPlayerCollider->Parent()->CastTo<Object3D>()->position += pushOut;
		}
	}
}

void CollisionManager::PlayerHitDoors()
{
	auto playerBodyCollider = mPlayerCollider->GetBodyCollider();

	for (const auto& dc : mDoorColliders)
	{
		// 押し出し
		Vec3 pushOut = Vec3::zero;
		if (dc->GetLeftCollider().IsTrigger(&playerBodyCollider, &pushOut))
		{
			mPlayerCollider->Parent()->CastTo<Object3D>()->position += pushOut;
		}
		if (dc->GetRightCollider().IsTrigger(&playerBodyCollider, &pushOut))
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
			auto flyblock = GameManager::GetInstance()->GetFlyBlock(fbc->Parent());

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
				if (flyblock->GetAttractVec().GetLength() == 0)
				{
					auto flyBlockObj3D = fbc->Parent()->CastTo<Object3D>();

					float posY = bc->GetBodyCollider().pos.y;
					float offsetY = bc->GetBodyCollider().scale.y + flyBlockObj3D->scale.y;
					flyBlockObj3D->position.y = posY + offsetY;
				}

				flyblock->ZeroGravity();
				flyblock->EndAttracting();
			}
		}
	}
}

void CollisionManager::FlyBlocksHitButtons()
{
	for (const auto& bc : mButtonColliders)
	{
		//ステージ内のボタン
		auto button = bc->Parent()->CastTo<Object3D>();
		auto stageButton = SceneManager::FindChildObject<StageButton>("StageButton", button);

		// 重力
		auto buttonBodyCollider = bc->GetBodyCollider();
		auto buttonFlameCollider = bc->GetFlameCollider();
		for (const auto& fbc : mFlyBlockColliders)
		{
			auto flyBlockDownCollider = fbc->GetDownCollider();

			if (flyBlockDownCollider.IsTrigger(&buttonBodyCollider))
			{
				// 飛ぶブロック
				auto flyBlock = GameManager::GetInstance()->GetFlyBlock(fbc->Parent());

				if (!flyBlock->GetIsAttracting())
				{
					if (flyBlock->GetGravity()->GetVelocity().y <= 0.f)
					{
						float posY = buttonBodyCollider.pos.y;
						float offsetY = (flyBlockDownCollider.scale.y * 2) + buttonBodyCollider.scale.y;

						fbc->Parent()->CastTo<Object3D>()->position.y = posY + offsetY;
						flyBlock->ZeroGravity();
					}
				}

				// ボタン凹ませる
				stageButton->BeginPushingButton();

				//同じタグを持つドアがあれば開ける
				for (auto& door : SceneManager::FindObjectsWithTag<Object3D>("StageDoor"))
				{
					std::string str = button->GetSameTag(*door->CastTo<IComponent>(), "DB");

					if (str.size())
					{
						ConsoleWindow::Log(str);
						auto linkDoor = SceneManager::FindChildObject<StageDoor>("StageDoor", door);
						linkDoor->OpenDoor();
					}
				}

				break;
			}
			else if (flyBlockDownCollider.IsTrigger(&buttonFlameCollider))
			{
				// 飛ぶブロック
				auto flyBlock = GameManager::GetInstance()->GetFlyBlock(fbc->Parent());

				if (!flyBlock->GetIsAttracting())
				{
					if (flyBlock->GetGravity()->GetVelocity().y <= 0.f)
					{
						float posY = buttonFlameCollider.pos.y;
						float offsetY = (flyBlockDownCollider.scale.y * 2) + buttonFlameCollider.scale.y;

						fbc->Parent()->CastTo<Object3D>()->position.y = posY + offsetY;
						flyBlock->ZeroGravity();
					}
				}

				break;
			}
		}
	}
}

void CollisionManager::FlyBlocksHitGlasses()
{
	for (const auto& fbc : mFlyBlockColliders)
	{
		auto flyBlockDownCollider = fbc->GetDownCollider();
		auto flyBlockMoveCollider = fbc->GetMoveCollider();

		for (const auto& gc : mGlassColliders)
		{
			auto flyblock = GameManager::GetInstance()->GetFlyBlock(fbc->Parent());

			// 押し出し
			Vec3 pushOut = Vec3::zero;
			if (gc->GetBodyCollider().IsTrigger(&flyBlockMoveCollider, &pushOut))
			{
				fbc->Parent()->CastTo<Object3D>()->position += pushOut;
				flyblock->EndAttracting();
			}

			// 重力
			if (gc->GetBodyCollider().IsTrigger(&flyBlockDownCollider) &&
				!flyblock->GetIsAttracting())
			{
				if (flyblock->GetAttractVec().GetLength() == 0)
				{
					auto flyBlockObj3D = fbc->Parent()->CastTo<Object3D>();

					float posY = gc->GetBodyCollider().pos.y;
					float offsetY = gc->GetBodyCollider().scale.y + flyBlockObj3D->scale.y;
					flyBlockObj3D->position.y = posY + offsetY;
				}

				flyblock->ZeroGravity();
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
			auto flyBlockTopCollider1 = fbc1->GetTopCollider();

			// ブロック2
			auto flyBlockBodyCollider2 = fbc2->GetBodyCollider();
			auto flyBlockMoveCollider2 = fbc2->GetMoveCollider();
			auto flyBlockDownCollider2 = fbc2->GetDownCollider();
			auto flyBlockTopCollider2 = fbc2->GetTopCollider();

			//自分
			auto flyBlock1 = GameManager::GetInstance()->GetFlyBlock(fbc1->Parent());
			//相手
			auto flyBlock2 = GameManager::GetInstance()->GetFlyBlock(fbc2->Parent());

			// 自分が引き寄せられていない時
			if (!flyBlock1->GetIsAttracting())
			{
				// 重力関連
				if (flyBlockTopCollider1.IsTrigger(&flyBlockDownCollider2))
				{
					if (flyBlock2->GetGravity()->GetVelocity().y <= 0.f)
					{
						flyBlock2->ZeroGravity();

						// 位置補正
						float posY = flyBlockTopCollider1.pos.y;
						float offsetY = flyBlockDownCollider2.scale.y * 5.f;
						fbc2->Parent()->CastTo<Object3D>()->position.y = posY + offsetY;
					}
				}
			}
			else
			{
				// 押し戻し
				Vec3 pushOut = Vec3::zero;
				if (flyBlockMoveCollider1.IsTrigger(&flyBlockBodyCollider2, &pushOut))
				{
					if (flyBlock1)
					{
						//Vec3 offset = pushOut.Norm() * 0.0001f;
						fbc1->Parent()->CastTo<Object3D>()->position += pushOut/* + offset*/;
						flyBlock1->EndAttracting();

						Vec3 hitPos =
						{ (flyBlockBodyCollider2.pos.x + flyBlockBodyCollider1.pos.x) / 2,
							(flyBlockBodyCollider2.pos.y + flyBlockBodyCollider1.pos.y) / 2 - 9,
							(flyBlockBodyCollider2.pos.z + flyBlockBodyCollider1.pos.z) / 2 };

						Vec3 rotaVel = flyBlockBodyCollider2.pos - flyBlockBodyCollider1.pos;

						OutputDebugStringA("mBlockCollHit!!");
						if (!mBlockCollEffect)
						{
							OutputDebugStringA("mBlockCollNull!!");
						}
						mBlockCollEffect->SetIsHit(true);
						mBlockCollEffect->SetHitBlockPos(hitPos);
						mBlockCollEffect->SetHitRota(rotaVel.GetNorm());

					}
				}
			}




			//// 重力
			//if (flyBlockDownCollider1.IsTrigger(&flyBlockBodyCollider2))
			//{
			//	////下のブロックの引き寄せベクトルをこちらにも適用
			//	//flyBlock1->SetAttractVec(flyBlock2->GetAttractVec() * 1.01f);

			//	if (flyBlock1->GetGravity()->GetVelocity().y <= 0.f)
			//	{
			//		flyBlock1->ZeroGravity();

			//		if (!flyBlock2->GetIsAttracting())
			//		{
			//			float posY = flyBlockBodyCollider2.pos.y;
			//			float offsetY = (flyBlockDownCollider1.scale.y * 2) + flyBlockBodyCollider2.scale.y;

			//			fbc1->Parent()->CastTo<Object3D>()->position.y = posY + offsetY;
			//		}
			//	}
			//}


		}
	}
}

void CollisionManager::FlyBlocksHitGoals()
{
	for (const auto& fbc : mFlyBlockColliders)
	{
		for (const auto& gc : mGoalColliders)
		{
			// 押し戻し
			Vec3 pushOut = Vec3::zero;

			auto goalBodyCollider = gc->GetBodyCollider();
			if (fbc->GetBodyCollider().IsTrigger(&goalBodyCollider, &pushOut))
			{
				FlyBlock* flyBlock = GameManager::GetInstance()->GetFlyBlock(fbc->Parent());

				if (flyBlock)
				{
					if (flyBlock->GetAttractVec().GetSquaredLength() != 0)
					{
						fbc->Parent()->CastTo<Object3D>()->position += pushOut;
						flyBlock->EndAttracting();
					}
				}
			}
		}
	}
}

void CollisionManager::FlyBlocksHitDoors()
{
	for (const auto& fbc : mFlyBlockColliders)
	{
		for (const auto& dc : mDoorColliders)
		{
			auto flyBlockBodyCollider = fbc->GetBodyCollider();

			// 押し出し
			Vec3 pushOut = Vec3::zero;
			if (dc->GetLeftCollider().IsTrigger(&flyBlockBodyCollider, &pushOut))
			{
				mPlayerCollider->Parent()->CastTo<Object3D>()->position += pushOut;
			}
			if (dc->GetRightCollider().IsTrigger(&flyBlockBodyCollider, &pushOut))
			{
				mPlayerCollider->Parent()->CastTo<Object3D>()->position += pushOut;
			}
		}
	}
}


float CollisionManager::CheckRayHitOtherDis(FlyBlockCollider* current)
{
	float minDis = 9999999.f;
	//float dis = 0.f;

	auto rayCollider = mViewCollider->GetRayCollider();

	// ブロックと
	for (const auto& bc : mBlockColliders)
	{
		auto bodyCollider = bc->GetBodyCollider();

		if (rayCollider.IsTrigger(&bodyCollider))
		{
			float dis = rayCollider.disToInter;
			if (dis < minDis)
			{
				minDis = dis;
			}
		}
	}

	// ボタンと
	for (const auto& bc : mButtonColliders)
	{
		auto bodyCollider = bc->GetBodyCollider();
		auto flameCollider = bc->GetFlameCollider();

		if (rayCollider.IsTrigger(&bodyCollider))
		{
			float dis = rayCollider.disToInter;
			if (dis < minDis)
			{
				minDis = dis;
			}
		}
		else if (rayCollider.IsTrigger(&flameCollider))
		{
			float dis = rayCollider.disToInter;
			if (dis < minDis)
			{
				minDis = dis;
			}
		}
	}

	// ドアと
	for (const auto& dc : mDoorColliders)
	{
		auto leftCollider = dc->GetLeftCollider();
		auto rightCollider = dc->GetRightCollider();

		if (rayCollider.IsTrigger(&leftCollider))
		{
			float dis = rayCollider.disToInter;
			if (dis < minDis)
			{
				minDis = dis;
			}
		}
		else if (rayCollider.IsTrigger(&rightCollider))
		{
			float dis = rayCollider.disToInter;
			if (dis < minDis)
			{
				minDis = dis;
			}
		}
	}

	// ゴールと
	for (const auto& gc : mGoalColliders)
	{
		auto bodyCollider = gc->GetBodyCollider();
		if (rayCollider.IsTrigger(&bodyCollider))
		{
			float dis = rayCollider.disToInter;
			if (dis < minDis)
			{
				minDis = dis;
			}
		}
	}

	// 他のブロック
	for (const auto& fbc : mFlyBlockColliders)
	{
		if (fbc == current)
		{
			continue;
		}

		auto bodyCollider = fbc->GetBodyCollider();
		if (rayCollider.IsTrigger(&bodyCollider))
		{
			float dis = rayCollider.disToInter;
			if (dis < minDis)
			{
				minDis = dis;
			}
		}
	}

	return minDis;
}

void CollisionManager::CheckRayHitWhFlyBlockAndWhWall()
{
	if (!GrayScale::GetIsGrayScale())
	{
		return;
	}

	for (const auto& fbc : mFlyBlockColliders)
	{
		auto fb = GameManager::GetInstance()->GetFlyBlock(fbc->Parent());

		if (fb)
		{
			fb->SetIsUpdate(true);
		}
	}

	for (const auto& fbc : mFlyBlockColliders)
	{
		auto fb = GameManager::GetInstance()->GetFlyBlock(fbc->Parent());

		/*if (fb->GetBlockType() == FlyBlock::BlockType::NORMAL)
		{
			continue;
		}*/

		auto fbObj = fbc->Parent()->CastTo<Object3D>();
		Vec3 pos = { fbObj->position.x,fbObj->position.y,fbObj->position.z };

		//白ブロックへのレイ（モデルとスケールで-1.0fとかが変わるので注意）
		std::vector<RayCollider> rays;
		RayCollider ray1; ray1.Setting(mViewCollider->GetPos(),
			(pos + Vec3{ -1.0f,1.0f,-1.0f } - mViewCollider->GetPos()).GetNorm(), 9999.0f);
		RayCollider ray2; ray2.Setting(mViewCollider->GetPos(),
			(pos + Vec3{ 1.0f,-1.0f,1.0f } - mViewCollider->GetPos()).GetNorm(), 9999.0f);
		RayCollider ray3; ray3.Setting(mViewCollider->GetPos(),
			(pos + Vec3{ 1.0f,1.0f,1.0f } - mViewCollider->GetPos()).GetNorm(), 9999.0f);
		RayCollider ray4; ray4.Setting(mViewCollider->GetPos(),
			(pos + Vec3{ 1.0f,-1.0f,-1.0f } - mViewCollider->GetPos()).GetNorm(), 9999.0f);

		RayCollider ray5; ray5.Setting(mViewCollider->GetPos(),
			(pos + Vec3{ -1.0f,-1.0f,-1.0f } - mViewCollider->GetPos()).GetNorm(), 9999.0f);
		RayCollider ray6; ray6.Setting(mViewCollider->GetPos(),
			(pos + Vec3{ -1.0f,-1.0f,1.0f } - mViewCollider->GetPos()).GetNorm(), 9999.0f);
		RayCollider ray7; ray7.Setting(mViewCollider->GetPos(),
			(pos + Vec3{ -1.0f,1.0f,1.0f } - mViewCollider->GetPos()).GetNorm(), 9999.0f);
		RayCollider ray8; ray8.Setting(mViewCollider->GetPos(),
			(pos + Vec3{ 1.0f,1.0f,-1.0f } - mViewCollider->GetPos()).GetNorm(), 9999.0f);

		rays.push_back(ray1);
		rays.push_back(ray2);
		rays.push_back(ray3);
		rays.push_back(ray4);
		rays.push_back(ray5);
		rays.push_back(ray6);
		rays.push_back(ray7);
		rays.push_back(ray8);


		//そのレイ上に白い壁もあるかどうか
		for (const auto& bc : mBlockColliders)
		{
			//白い壁じゃなければ
			if (bc->Parent()->CastTo<Object3D>()->texType != 2)
			{
				continue;
			}

			ConsoleWindow::Log("WHITE");

			auto bBodyCollider = bc->GetBodyCollider();

			for (auto& ray : rays)
			{
				if (ray.IsTrigger(&bBodyCollider))
				{
					fb->SetIsUpdate(false);

					ConsoleWindow::Log("BAKAKAKAKAKKAKAKA");
				
					break;
				}
			}
		}
	}
}

void CollisionManager::RecursiveAttracting(
	FlyBlockCollider* current, const std::vector<FlyBlockCollider*>& colliders)
{
	std::vector<FlyBlockCollider*> check = colliders;
	check.erase(std::remove(check.begin(), check.end(), current), check.end());

	for (const auto& target : check)
	{
		// 同じなら
		if (current == target)
		{
			continue;
		}

		// 仮に移動させて当たるがどうかをチェックする用
		if (CheckHitOther(target))
		{
			// 当たっていたら移動させない
			break;
		}

		// 現在移動させようとしてるやつ
		auto currentTop = current->GetTopCollider();

		// 上に乗ってるやつ
		auto targetDown = target->GetDownCollider();

		if (currentTop.IsTrigger(&targetDown))
		{
			auto currentFlyBlock = GameManager::GetInstance()->GetFlyBlock(current->Parent());
			auto targetFlyBlock = GameManager::GetInstance()->GetFlyBlock(target->Parent());

			Vec3 vec = currentFlyBlock->GetAttractedDir();
			float dis = Vec3::Distance(currentFlyBlock->GetBeginPos(), currentFlyBlock->GetEndPos());

			Vec3 ePos = Vec3(target->Parent()->CastTo<Object3D>()->position) + vec * dis;
			targetFlyBlock->BeginAttracting(ePos);

			target->IsMoveing();

			RecursiveAttracting(target, check);
		}
	}
}

bool CollisionManager::CheckHitOther(FlyBlockCollider* current)
{
	//SphereCollider coll;
	Vec3 pos = current->Parent()->CastTo<Object3D>()->position;
	float r = Vec3(current->Parent()->CastTo<Object3D>()->scale).GetMaxElement() * 0.5f;

	Vec3 vec = -mViewCollider->GetRayCollider().r.ray.Norm() * r * 2.5f;

	float radian = ConvertAngleToRadian(10);

	Vec3 offset1 = Quaternion::AnyAxisRotation(vec, Vec3::up, radian);
	coll1.Setting(pos + offset1, r);

	Vec3 offset2 = Quaternion::AnyAxisRotation(vec, Vec3::up, -radian);
	coll2.Setting(pos + offset2, r);

	// 飛ぶブロック
	for (const auto& fbc : mFlyBlockColliders)
	{
		if (current == fbc)
		{
			continue;
		}

		if (fbc->GetBodyCollider().IsTrigger(&coll1) ||
			fbc->GetBodyCollider().IsTrigger(&coll2))
		{
			return true;
		}
	}

	// ガラス
	for (const auto& gc : mGlassColliders)
	{
		if (gc->GetBodyCollider().IsTrigger(&coll1) ||
			gc->GetBodyCollider().IsTrigger(&coll2))
		{
			return true;
		}
	}

	return false;
}

RegisterScriptBody(CollisionManager);
