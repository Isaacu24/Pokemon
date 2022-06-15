#include "Green.h"
#include <GameEngine/GameEngineImage.h>
#include <GameEngine/GameEngineImageManager.h>
#include <GameEngine/GameEngineRenderer.h>
#include <GameEngine/GameEngineCollision.h>

#include "ContentEnum.h"
#include "PlayerRed.h"
#include "NPCBase.h"
#include "Oak.h"
#include "PokemonInfoManager.h"
#include "RoomTileMap4.h"
#include "InteractionText.h"
#include "BattleStartUI.h"

Green* Green::NPCGreen = nullptr;
Green::Green() 
	: IsRedSelectFinish_(false)
	, IsGreenMove_(false)
	, IsSelectDialog_(false)
	, IsBattleEnd_(false)
{
	NPCGreen = this;
}

Green::~Green() 
{
}

void Green::Start()
{
	SetBattleNPC(true);
	PushPokemon(PokemonInfoManager::GetInst().CreatePokemon("Bulbasaur"));
	
	NPCCollision_ = CreateCollision("NPCColBox", { 60,60 });
	NPCUpCollision_ = CreateCollision("GreenDirZColBox", { 20,5 }, { 0,-32 });
	NPCDownCollision_ = CreateCollision("GreenDirZColBox", { 20,5 }, { 0,32 });
	NPCRightCollision_ = CreateCollision("GreenDirZColBox", { 5,20 }, { 32,0 });
	NPCLeftCollision_ = CreateCollision("GreenDirZColBox", { 5,20 }, { -32,0 });
	{
		GameEngineImage*
		// Idle
		Image = GameEngineImageManager::GetInst()->Find("Green_Idle_Up.bmp");
		Image->Cut({ 56,80 });
		Image = GameEngineImageManager::GetInst()->Find("Green_Idle_Down.bmp");
		Image->Cut({ 60,80 });
		Image = GameEngineImageManager::GetInst()->Find("Green_Idle_Right.bmp");
		Image->Cut({ 56,80 });
		Image = GameEngineImageManager::GetInst()->Find("Green_Idle_Left.bmp");
		Image->Cut({ 56,80 });

		// Walk
		Image = GameEngineImageManager::GetInst()->Find("Green_Walk_Up.bmp");
		Image->CutCount(3, 1);
		Image = GameEngineImageManager::GetInst()->Find("Green_Walk_Down.bmp");
		Image->CutCount(3, 1);
		Image = GameEngineImageManager::GetInst()->Find("Green_Walk_Right.bmp");
		Image->CutCount(3, 1);
		Image = GameEngineImageManager::GetInst()->Find("Green_Walk_Left.bmp");
		Image->CutCount(3, 1);
	}

	NPCRender_ = CreateRenderer(static_cast<int>(RenderOrder::Player));
	NPCRender_->CreateAnimation("Green_Idle_Up.bmp", "IdleUp", 0, 0, 0.0f, false);
	NPCRender_->CreateAnimation("Green_Idle_Down.bmp", "IdleDown", 0, 0, 0.0f, false);
	NPCRender_->CreateAnimation("Green_Idle_Right.bmp", "IdleRight", 0, 0, 0.0f, false);
	NPCRender_->CreateAnimation("Green_Idle_Left.bmp", "IdleLeft", 0, 0, 0.0f, false);

	NPCRender_->CreateAnimation("Green_Walk_Up.bmp", "WalkUp", 0, 2, 0.1f, true);
	NPCRender_->CreateAnimation("Green_Walk_Down.bmp", "WalkDown", 0, 2, 0.1f, true);
	NPCRender_->CreateAnimation("Green_Walk_Right.bmp", "WalkRight", 0, 2, 0.1f, true);
	NPCRender_->CreateAnimation("Green_Walk_Left.bmp", "WalkLeft", 0, 2, 0.1f, true);

	NPCRender_->ChangeAnimation("IdleDown");
	NPCRender_->SetPivot({ 0,-15 });

	CurrentTileMap_ = RoomTileMap4::GetInst();
	SetPosition(CurrentTileMap_->GetWorldPostion(5, 2));

	for (int i = 0; i < 20; i++)
	{
		IsStart_.push_back(false);
	}
}

void Green::Update()
{
	NPCMoveAnim();
	RedSelectPokemonAfter();
	RedGreenBattle();
	BattleAfter();
}

void Green::LevelChangeStart(GameEngineLevel* _PrevLevel)
{
	if (nullptr != dynamic_cast<WorldMapLevel*>(GetLevel()))
	{
		if (true == IsStart_[7])
		{
			// ������ ��� + �ⱸ�� ������� 
			if (false == IsBattleEnd_)
			{
				IsBattleEnd_ = true;
			}
		}

		if (true == IsStart_[11])
		{
			//SetPosition(CurrentTileMap_->GetWorldPostion(5, 2));
			//NPCRender_->On();
		}
	}
}

void Green::LevelChangeEnd(GameEngineLevel* _NextLevel)
{
}

void Green::RedSelectPokemonAfter()
{
	if (true == IsRedSelectFinish_)
	{
		//PlayerRed::MainRed_->SetInteraction(true);
		if (false == IsStart_[0])
		{
			IsStart_[0] = true;
			NPCMoveDir(NPCDir::Right, 1);	//6,3
		}

		if (false == IsStart_[1] && GetAccTime() >= NextDirMoveTimer_)
		{
			IsStart_[1] = true;
			NPCMoveDir(NPCDir::Down, 2);	//6,3 - 6,4
		}

		if (false == IsStart_[2] && GetAccTime() >= NextDirMoveTimer_)
		{
			IsStart_[2] = true;
			NPCMoveDir(NPCDir::Right, 2);	//7,4 - 8,4
		}

		if (false == IsStart_[3] && GetAccTime() >= NextDirMoveTimer_)
		{
			IsStart_[3] = true;
			NPCRender_->ChangeAnimation("IdleUp");
		}

		if (false == IsStart_[4] && GetAccTime() >= NextDirMoveTimer_)
		{
			IsStart_[4] = true;
			RoomTileMap4::GetInst()->Pokeball0->Off();
			InteractionText* TmpText = GetLevel()->CreateActor<InteractionText>();
			TmpText->SetPosition(PlayerRed::MainRed_->GetPosition());
			TmpText->AddText("GREEN: I'll take this one, then!");
			TmpText->AddText("");
			TmpText->AddText("GREEN received the BULBASAUR");
			TmpText->AddText("from PROF. OAK!");
			TmpText->Setting();

			// Ÿ��üũ bool true�� ��ȯ(������ ������ �� ��)
		}
	}
}

void Green::RedGreenBattle()
{
	if (true == IsGreenMove_)
	{
		// RedCurrentIndex_ = 5 => left 3 / RedCurrentIndex_ = 6 => left 2 / RedCurrentIndex_ = 7 => left 1
		if (5 == PlayerRed::MainRed_->GetRedCurrentIndex().x)
		{
			if (false == IsStart_[5])
			{
				IsStart_[5] = true;
				NPCMoveDir(NPCDir::Left, 3);
			}

			if (false == IsStart_[6] && GetAccTime() >= NextDirMoveTimer_)
			{
				IsStart_[6] = true;
				NPCMoveDir(NPCDir::Down, 1);

				IsGreenMove_ = false;				
			}

			if (false == IsStart_[7] && false == IsGreenMove_)
			{
				IsStart_[7] = true;

				BattleStartUI* TmpBattleStartUI = GetLevel()->CreateActor<BattleStartUI>();
				TmpBattleStartUI->ChangeToBattleLevel(BattleNpcType::Rival);
			}
		}

		if (6 == PlayerRed::MainRed_->GetRedCurrentIndex().x)
		{
			if (false == IsStart_[5])
			{
				IsStart_[5] = true;
				NPCMoveDir(NPCDir::Left, 2);
			}

			if (false == IsStart_[6] && GetAccTime() >= NextDirMoveTimer_)
			{
				IsStart_[6] = true;
				NPCMoveDir(NPCDir::Down, 1);

				IsGreenMove_ = false;
			}

			if (false == IsStart_[7] && false == IsGreenMove_)
			{
				IsStart_[7] = true;

				BattleStartUI* TmpBattleStartUI = GetLevel()->CreateActor<BattleStartUI>();
				TmpBattleStartUI->ChangeToBattleLevel(BattleNpcType::Rival);
			}
		}

		if (7 == PlayerRed::MainRed_->GetRedCurrentIndex().x)
		{
			if (false == IsStart_[5])
			{
				IsStart_[5] = true;
				NPCMoveDir(NPCDir::Left, 1);
			}

			if (false == IsStart_[6] && GetAccTime() >= NextDirMoveTimer_)
			{
				IsStart_[6] = true;
				NPCMoveDir(NPCDir::Down, 1);

				IsGreenMove_ = false;
			}

			if (false == IsStart_[7] && false == IsGreenMove_)
			{
				IsStart_[7] = true;

				BattleStartUI* TmpBattleStartUI = GetLevel()->CreateActor<BattleStartUI>();
				TmpBattleStartUI->ChangeToBattleLevel(BattleNpcType::Rival);
			}
		}
	}
}

void Green::BattleAfter()
{
	if (true == IsBattleEnd_)
	{
		if (5 == PlayerRed::MainRed_->GetRedCurrentIndex().x)
		{
			if (false == IsStart_[8])
			{
				IsStart_[8] = true;
				NPCMoveDir(NPCDir::Right, 1);
			}

			if (false == IsStart_[9] && GetAccTime() >= NextDirMoveTimer_)
			{
				IsStart_[9] = true;
				NPCMoveDir(NPCDir::Down, 3);
			}

			if (false == IsStart_[10] && GetAccTime() >= NextDirMoveTimer_)
			{
				IsStart_[10] = true;

				NPCMoveDir(NPCDir::Down, 2);
			}

			if (false == IsStart_[11] && GetAccTime() >= NextDirMoveTimer_)
			{
				IsStart_[11] = true;

				SetPosition({ -6500, -7000 });
				//NPCRender_->Off();
				//NPCCollision_->Off();
			}
		}

		if (6 == PlayerRed::MainRed_->GetRedCurrentIndex().x)
		{
			if (false == IsStart_[8])
			{
				IsStart_[8] = true;
				NPCMoveDir(NPCDir::Right, 1);
			}

			if (false == IsStart_[9] && GetAccTime() >= NextDirMoveTimer_)
			{
				IsStart_[9] = true;
				NPCMoveDir(NPCDir::Down, 3);
			}

			if (false == IsStart_[10] && GetAccTime() >= NextDirMoveTimer_)
			{
				IsStart_[10] = true;

				NPCMoveDir(NPCDir::Left, 1);
			}

			if (false == IsStart_[11] && GetAccTime() >= NextDirMoveTimer_)
			{
				IsStart_[11] = true;

				NPCMoveDir(NPCDir::Down, 2);
			}

			if (false == IsStart_[12] && GetAccTime() >= NextDirMoveTimer_)
			{
				IsStart_[12] = true;

				SetPosition({ -6500, -7000 });
				//NPCRender_->Off();
				//NPCCollision_->Off();
			}
		}

		if (7 == PlayerRed::MainRed_->GetRedCurrentIndex().x)
		{
			if (false == IsStart_[8])
			{
				IsStart_[8] = true;
				NPCMoveDir(NPCDir::Left, 1);
			}

			if (false == IsStart_[9] && GetAccTime() >= NextDirMoveTimer_)
			{
				IsStart_[9] = true;
				NPCMoveDir(NPCDir::Down, 3);
			}

			if (false == IsStart_[10] && GetAccTime() >= NextDirMoveTimer_)
			{
				IsStart_[10] = true;

				NPCMoveDir(NPCDir::Down, 2);
			}

			if (false == IsStart_[11] && GetAccTime() >= NextDirMoveTimer_)
			{
				IsStart_[11] = true;

				SetPosition({ -6500, -7000 });
				//NPCRender_->Off();
				//NPCCollision_->Off();
			}
		}
	}
}

