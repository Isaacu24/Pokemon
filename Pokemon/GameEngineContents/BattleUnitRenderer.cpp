#include "BattleUnitRenderer.h"
#include "GameEngineBase/GameEngineWindow.h"
#include "GameEngineBase/GameEngineTime.h"
#include <GameEngine/GameEngineImageManager.h>
#include "BattleLevel.h"
#include <tchar.h>
#include <GameEngine/GameEngine.h>
#include <GameEngineBase/GameEngineInput.h>
#include "BattleInterface.h"

GameEngineRenderer* BattleUnitRenderer::PlayerRenderer_ = nullptr;

BattleUnitRenderer::BattleUnitRenderer()
	: PlayerCurrentPokemon_(nullptr)
	, PoeCurrentPokemon_(nullptr)
	, OpponentRenderer_(nullptr)//병문씨 도움
	, FirstMove(true)
	, PlayerPokemonPos_({ -220, 63 })
	, OpponentPokemonPos_({ -450, -105 })//200,-105
	, PlayerRendererPos_({ 480,31 })
	, OpponentRenderer_Pos_()
	, MoveSpeed(900.0f)//원래는200
	, PlayerStop(false)
	, TimeCheck(0.0f)
	, MonsterBallOP(nullptr)
	, MyTackleEffect(nullptr)
	, BattleDataR_(nullptr)
	, Level_(nullptr)
	, MyMoveTime(0.0f)
	, MyTurnEnd(true)
	, Angle(0.0f)
	, Fighting(false)
	, MyWaterGunEffect(nullptr)
	, BallLerp(0.0f)
	, BattleInter(nullptr)
	, EnemyTackleEffect(nullptr)
	, PlayerTime_(0.0f)
	, EffectX(-105.0f)
	, EffectY(20.0f)
	, Rock1(nullptr)
	, Rock2(nullptr)
	, Rock3(nullptr)
	, Rock4(nullptr)
	, X(nullptr)
	, EnemyTurnEnd(true)
	, Rock1Pivot({ -220.f, -100.0f })
	, Rock2Pivot({ -160.f,-100.0f })
	, Rock3Pivot({ -280.f, -100.0f })
	, Rock4Pivot({ -240.f,-100.0f })
	, RockSpeed(600.0f)
	, Rock1End(false)
	, Rock2End(false)
	, Rock3End(false)
	, Rock4End(false)
	, AnimationEndTime(0.0f)
	, CatchBallTime(0.0f)
	, CatchBallOpen(nullptr)
	, Alpha_Time(0.0f)
	, MyCatchEnd(true)
	, SkillName_(SkillName::None)
	, BallFall(-170.0f)
	, FallCheck(false)
	, PlayerTime_2(0.0f)
	, BallFallTime(0.0f)
	, BallX(0.0f)
	, BallY(0.0f)
	, IsCatch(false)
	, MonsterBallCH(nullptr)
	, MyGrowl1(nullptr)
	, MyGrowl2(nullptr)
	, MyGrowl3(nullptr)
	, EnemyGrowl1(nullptr)
	, EnemyGrowl2(nullptr)
	, EnemyGrowl3(nullptr)
	, Debug_(false)
	, EnemyScratchEffect(nullptr)
{
}	
BattleUnitRenderer::~BattleUnitRenderer() 
{
}

void BattleUnitRenderer::Start()
{
	Level_ = dynamic_cast<BattleLevel*>(GetLevel());

	SetPosition({ GameEngineWindow::GetScale().Half() });

	GameEngineInput::GetInst()->CreateKey("BattleStatuDebug", VK_NUMPAD8);
	{
		GameEngineImage* Image = GameEngineImageManager::GetInst()->Find("Player.bmp");
		Image->CutCount(1, 1);
	}
	{
		GameEngineImage* Image = GameEngineImageManager::GetInst()->Find("PlayerAnimation.bmp");
		Image->CutCount(5, 1);
	}
	{
		GameEngineImage* Image = GameEngineImageManager::GetInst()->Find("BallRoll.bmp");
		Image->CutCount(6, 1);
	}
	{
		GameEngineImage* Image = GameEngineImageManager::GetInst()->Find("WaterGun4.bmp");
		Image->CutCount(3, 1);
	}
	{
		GameEngineImage* Image = GameEngineImageManager::GetInst()->Find("ShellHide.bmp");
		Image->CutCount(8, 1);
	}
	{
		GameEngineImage* Image = GameEngineImageManager::GetInst()->Find("SquirtleB.bmp");
		Image->CutCount(1, 1);
	}
	{
		GameEngineImage* Image = GameEngineImageManager::GetInst()->Find("MonsterBall_Open4.bmp");
		Image->CutCount(2, 1);
	}
	{
		GameEngineImage* Image = GameEngineImageManager::GetInst()->Find("Catch4.bmp");
		Image->CutCount(4, 1);
	}
	{
		GameEngineImage* Image = GameEngineImageManager::GetInst()->Find("Growl1x4.bmp");
		Image->CutCount(2, 1);
	}
	{
		GameEngineImage* Image = GameEngineImageManager::GetInst()->Find("Growl2x4.bmp");
		Image->CutCount(2, 1);
	}
	{
		GameEngineImage* Image = GameEngineImageManager::GetInst()->Find("Growl3x4.bmp");
		Image->CutCount(2, 1);
	}
	{
		GameEngineImage* Image = GameEngineImageManager::GetInst()->Find("EGrowl1x4.bmp");
		Image->CutCount(2, 1);
	}
	{
		GameEngineImage* Image = GameEngineImageManager::GetInst()->Find("EGrowl2x4.bmp");
		Image->CutCount(2, 1);
	}
	{
		GameEngineImage* Image = GameEngineImageManager::GetInst()->Find("EGrowl3x4.bmp");
		Image->CutCount(2, 1);
	}
	{
		GameEngineImage* Image = GameEngineImageManager::GetInst()->Find("Scratch4.bmp");
		Image->CutCount(5, 1);
	}

}

void BattleUnitRenderer::Update()
{
	TimeCheck += (GameEngineTime::GetDeltaTime() * 2.0f);
	if (FirstMove == true)
	{
		if (BattleDataR_->IsWild())
		{
			Opening();
		}
		else
		{
			TrainerOpening1();
		}
	}
	if (BattleDataR_->IsWild())
	{
		Opening2();
	}
	else
	{
		TrainerOpening2();
	}


	if (SkillName_ != SkillName::None && (MyTurnEnd == false || EnemyTurnEnd == false || MyCatchEnd == false))
	{
		Level_->DoingSkillAnimation_ = true;
		switch (SkillName_)
		{
		case SkillName::Tackle:
			Tackle();
			break;
		case SkillName::TailWhipMove:
			TailWhipMove();
			break;
		case SkillName::WaterGun:
			WaterGun();			
			break;
		case SkillName::ShellHide:
			ShellHide();
			break;
		case SkillName::EnemyRock:
			EnemyRock();
			break;
		case SkillName::EnemyTackle:
			EnemyTackle();
			break;
		case SkillName::Catch:
			Catch();
			break;
		case SkillName::EnemyGrowl:
			EnemyGrowl();
			break;
		case SkillName::MyGrowl:
			MyGrowl();
			break;
		case SkillName::EnemyScratch:
			EnemyScratch();
			break;
		default:
			break;
		}

		if (SkillName_ != SkillName::None && (MyTurnEnd == true && EnemyTurnEnd == true && MyCatchEnd == true))
		{
			SkillName_ = SkillName::None;
			Level_->DoingSkillAnimation_ = false;
		}
	}

	if (GameEngineInput::GetInst()->IsDown("BattleStatuDebug"))
	{
		Debug_ = !Debug_;
	}


}



void BattleUnitRenderer::Render()
{
	if (BattleInter->ChildUI_ == nullptr && Debug_ == true)
	{
		ShowDebugValue();
	}
}

void BattleUnitRenderer::ShowDebugValue()
{
	Pokemon* PlayerPokemon = BattleDataR_->GetCurrentPlayerPokemon()->GetPokemon();
	Pokemon* PoePokemon = BattleDataR_->GetCurrentPoePokemon()->GetPokemon();


	// Play Hp
	{
		char Text[10] = { 0 };
		sprintf_s(Text, "HP : %d", PlayerPokemon->GetInfo()->GetHp());
		TextOut(GameEngine::BackBufferDC(), PlayerCurrentPokemon_->GetSortingPivot().ix()
			, PlayerCurrentPokemon_->GetSortingPivot().iy(), Text, static_cast<int>(strlen(Text)));
	}
	// Play Att
	{
		char Text[10] = { 0 };
		sprintf_s(Text, "Att : %d", PlayerPokemon->GetInfo()->GetAtt());
		TextOut(GameEngine::BackBufferDC(), PlayerCurrentPokemon_->GetSortingPivot().ix()
			, PlayerCurrentPokemon_->GetSortingPivot().iy() + 15, Text, static_cast<int>(strlen(Text)));
	}
	// Play Def
	{
		char Text[10] = { 0 };
		sprintf_s(Text, "Def : %d", PlayerPokemon->GetInfo()->GetDef());
		TextOut(GameEngine::BackBufferDC(), PlayerCurrentPokemon_->GetSortingPivot().ix()
			, PlayerCurrentPokemon_->GetSortingPivot().iy() + 30, Text, static_cast<int>(strlen(Text)));
	}
	// Play Speed
	{
		char Text[15] = { 0 };
		sprintf_s(Text, "Speed : %d", PlayerPokemon->GetInfo()->GetSpeed());
		TextOut(GameEngine::BackBufferDC(), PlayerCurrentPokemon_->GetSortingPivot().ix()
			, PlayerCurrentPokemon_->GetSortingPivot().iy() + 45, Text, static_cast<int>(strlen(Text)));
	}

	for (int i = 0; i < 6; i++)
	{
		int Rank = BattleDataR_->GetCurrentPlayerPokemon()->CurrentRank_[(static_cast<PokemonAbility>(i))];
		std::string RankStr = BattleInter->AbilityString(static_cast<PokemonAbility>(i));
		if (Rank != 0)
		{
			char Text[15] = { 0 };
			sprintf_s(Text, "%c%c%c : %d", RankStr[0], RankStr[1], RankStr[2], Rank);
			TextOut(GameEngine::BackBufferDC(), PlayerCurrentPokemon_->GetSortingPivot().ix()
				, PlayerCurrentPokemon_->GetSortingPivot().iy() + 60 + (15 * i), Text, static_cast<int>(strlen(Text)));
		}
	}



	if (PoeCurrentPokemon_ != nullptr)
	{
		// Poe Hp
		{
			char Text[10] = { 0 };
			if (PoePokemon->GetInfo()->GetHp() >= 0)
			{
				sprintf_s(Text, "HP : %d", PoePokemon->GetInfo()->GetHp());
				TextOut(GameEngine::BackBufferDC(), PoeCurrentPokemon_->GetSortingPivot().ix()
					, PoeCurrentPokemon_->GetSortingPivot().iy(), Text, static_cast<int>(strlen(Text)));
			}

		}
		// Poe Att
		{
			char Text[10] = { 0 };
			sprintf_s(Text, "Att : %d", PoePokemon->GetInfo()->GetAtt());
			TextOut(GameEngine::BackBufferDC(), PoeCurrentPokemon_->GetSortingPivot().ix()
				, PoeCurrentPokemon_->GetSortingPivot().iy() + 15, Text, static_cast<int>(strlen(Text)));
		}
		// Poe Def
		{
			char Text[10] = { 0 };
			sprintf_s(Text, "Def : %d", PoePokemon->GetInfo()->GetDef());
			TextOut(GameEngine::BackBufferDC(), PoeCurrentPokemon_->GetSortingPivot().ix()
				, PoeCurrentPokemon_->GetSortingPivot().iy() + 30, Text, static_cast<int>(strlen(Text)));
		}
		// Poe Speed
		{
			char Text[15] = { 0 };
			sprintf_s(Text, "Speed : %d", PoePokemon->GetInfo()->GetSpeed());
			TextOut(GameEngine::BackBufferDC(), PoeCurrentPokemon_->GetSortingPivot().ix()
				, PoeCurrentPokemon_->GetSortingPivot().iy() + 45, Text, static_cast<int>(strlen(Text)));
		}

		for (int i = 0; i < 6; i++)
		{
			int Rank = BattleDataR_->GetCurrentPoePokemon()->CurrentRank_[(static_cast<PokemonAbility>(i))];
			std::string RankStr = BattleInter->AbilityString(static_cast<PokemonAbility>(i));
			if (Rank != 0)
			{
				char Text[15] = { 0 };
				sprintf_s(Text, "%c%c%c : %d", RankStr[0], RankStr[1], RankStr[2], Rank);
				TextOut(GameEngine::BackBufferDC(), PoeCurrentPokemon_->GetSortingPivot().ix()
					, PoeCurrentPokemon_->GetSortingPivot().iy() + 60 + (15 * i), Text, static_cast<int>(strlen(Text)));
			}
		}
	}

}

void BattleUnitRenderer::SetPoeRenderer()
{
	BattleDataR_ = Level_->GetBattleData();
	if (PoeCurrentPokemon_ == nullptr)
	{
		PoeCurrentPokemon_ = CreateRenderer(BattleDataR_->GetCurrentPoePokemon()->GetPokemon()->GetInfo()->GetMyBattleFront()
			, 3, RenderPivot::CENTER, OpponentPokemonPos_);
	}
}


void BattleUnitRenderer::LevelChangeStart(GameEngineLevel* _PrevLevel)
{

	if (BattleInter == nullptr)
	{
		BattleInter = dynamic_cast<BattleInterface*>(GetLevel()->FindActor("BattleInterface"));
	}
	FirstMove = true;
	Fighting = false;
	MoveSpeed = 900.0f;//원래는200
	RockSpeed = 300.0f;
	CatchBallTime = 0.0f;
	IsCatch = false;

	BattleDataR_ = Level_->GetBattleData();
	if (nullptr == PlayerCurrentPokemon_)
	{
		//푸키먼
		PlayerCurrentPokemon_ = CreateRenderer(BattleDataR_->GetCurrentPlayerPokemon()->GetPokemon()->GetInfo()->GetMyBattleBack()
			, 3, RenderPivot::CENTER, PlayerPokemonPos_);
		PlayerCurrentPokemon_->CreateAnimation("SquirtleB.bmp", "Idle", 0, 0, 0.0f, false);
		PlayerCurrentPokemon_->CreateAnimation("ShellHide.bmp", "ShellHide", 0, 7, 0.1f, false);

	}
	if (PoeCurrentPokemon_ == nullptr)
	{
		PoeCurrentPokemon_ = CreateRenderer(BattleDataR_->GetCurrentPoePokemon()->GetPokemon()->GetInfo()->GetMyBattleFront()
			, 3, RenderPivot::CENTER, OpponentPokemonPos_);
	}

	if (PlayerRenderer_ == nullptr)
	{
		//플레이어
		PlayerRenderer_ = CreateRenderer("Player.bmp", 4, RenderPivot::CENTER, PlayerRendererPos_);
		PlayerRenderer_->CreateAnimation("Player.bmp", "Stop", 0, 0, 0.1f, false);
		PlayerRenderer_->CreateAnimation("PlayerAnimation.bmp", "Go", 0, 4, 0.1f, false);

		//상대 트레이너
		//김예나 : 임시로 라이벌 이미지를 넣었어요 상황에 따라 야생 포켓몬처럼 다른 랜더러 뜨게 하면 될거같아요


		//볼
		MonsterBallOP = CreateRenderer("MonsterBall4.bmp", 4);
		MonsterBallOP->CreateAnimation("BallRoll.bmp", "BallRoll", 0, 5, 0.05f, true);
		MonsterBallOP->CreateAnimation("BallRoll.bmp", "Ball", 0, 0, 0.05f, false);
		MonsterBallOP->CreateAnimation("Catch4.bmp", "Catch", 0, 3, 0.2f, false);

		MonsterBallCH = CreateRenderer("MonsterBall4.bmp", 4);
		MonsterBallCH->CreateAnimation("BallRoll.bmp", "BallRoll", 0, 5, 0.05f, true);
		MonsterBallCH->CreateAnimation("BallRoll.bmp", "Ball", 0, 0, 0.05f, false);
		MonsterBallCH->CreateAnimation("Catch4.bmp", "Catch", 0, 3, 0.2f, false);
		// 볼 그냥 도는걸로 했는데 초반에 안도는거 하고싶으면 위에 플레이어 처럼 따로 생성필요

		CatchBallOpen = CreateRenderer("MonsterBall_Open4.bmp", 4);
		CatchBallOpen->CreateAnimation("MonsterBall_Open4.bmp", "Open", 0, 1, 0.3f, false);

		MyWaterGunEffect = CreateRenderer("WaterGun4.bmp", 4);
		MyWaterGunEffect->CreateAnimation("WaterGun4.bmp", "WaterGun", 0, 2, 0.1f, false);
		MyWaterGunEffect->CreateAnimation("WaterGun4.bmp", "Water", 0, 0, 0.1f, false);
		MyTackleEffect = CreateRenderer("Tackle4.bmp", 4);

		MyGrowl1 = CreateRenderer("Growl1x4.bmp", 4);
		MyGrowl1->CreateAnimation("Growl1x4.bmp", "Growl1", 0, 1, 0.2f, true);
		MyGrowl2 = CreateRenderer("Growl2x4.bmp", 4);
		MyGrowl2->CreateAnimation("Growl2x4.bmp", "Growl2", 0, 1, 0.2f, true);
		MyGrowl3 = CreateRenderer("Growl3x4.bmp", 4);
		MyGrowl3->CreateAnimation("Growl3x4.bmp", "Growl3", 0, 1, 0.2f, true);

		EnemyGrowl1 = CreateRenderer("EGrowl1x4.bmp", 4);
		EnemyGrowl1->CreateAnimation("EGrowl1x4.bmp", "Growl1", 0, 1, 0.2f, true);
		EnemyGrowl2 = CreateRenderer("EGrowl2x4.bmp", 4);
		EnemyGrowl2->CreateAnimation("EGrowl2x4.bmp", "Growl2", 0, 1, 0.2f, true);
		EnemyGrowl3 = CreateRenderer("EGrowl3x4.bmp", 4);
		EnemyGrowl3->CreateAnimation("EGrowl3x4.bmp", "Growl3", 0, 1, 0.2f, true);

		EnemyScratchEffect = CreateRenderer("Scratch4.bmp",4);
		EnemyScratchEffect->CreateAnimation("Scratch4.bmp", "Scratch", 0, 4, 0.1f, false);
		EnemyScratchEffect->CreateAnimation("Scratch4.bmp", "ScratchStart", 0, 0, 0.2f, false);

		Rock2 = CreateRenderer("Rock4.bmp", 4);
		Rock3 = CreateRenderer("Rock4.bmp", 4);
		Rock4 = CreateRenderer("Rock4.bmp", 2);//푸키먼 뒤로 넘어가는 돌
		Rock1 = CreateRenderer("Rock4.bmp", 4);
		X = CreateRenderer("X4.bmp", 4);

		Rock1Pivot = { -220.f,-100.0f };
		Rock2Pivot = { -160.f,-100.0f };
		Rock3Pivot = { -280.f,-100.0f };
		Rock4Pivot = { -240.f,-100.0f };
		Rock1->SetPivot(Rock1Pivot);
		Rock2->SetPivot(Rock2Pivot);
		Rock3->SetPivot(Rock3Pivot);
		Rock4->SetPivot(Rock4Pivot);
		X->SetPivot({ -200.0f,100.0f });
	}

	if (BattleDataR_->IsWild() == true) // 여기서 야생전투인지 구분하는곳
	{

	}
	else if(OpponentRenderer_ == nullptr)
	{
		if (BattleDataR_->GetNameString() == "Green")
		{
			OpponentRenderer_ = CreateRenderer("Rival_Battle.bmp", 3, RenderPivot::CENTER, OpponentPokemonPos_);
			OpponentRenderer_->Off();
			OpponentRenderer_->SetPivot({ -450.0f,-155.0f });
		}
		else if (BattleDataR_->GetNameString() == "Brock")
		{
			OpponentRenderer_ = CreateRenderer("Ung_Battle.bmp", 3, RenderPivot::CENTER, OpponentPokemonPos_);
			OpponentRenderer_->Off();
			OpponentRenderer_->SetPivot({ -450.0f,-155.0f });
		}
	
	}
	
	// 장병문 : 처음 한번만 만들기 Start에서 하는게 더 좋아보이는데 PlayerCurrentPokemon_ 올리면 문제생겨서 일단 여기둠
	// 추후에 수정필요할듯



	PlayerRenderer_->On();
	PlayerRenderer_->ChangeAnimation("Stop");
	PlayerCurrentPokemon_->Off();
	PlayerCurrentPokemon_->SetPivot(PlayerPokemonPos_);
	PoeCurrentPokemon_->On();
	MonsterBallOP->Off();
	MonsterBallOP->SetPivot({ -220.0f,-30.0f });
	MonsterBallCH->Off();
	MonsterBallCH->SetPivot({ -220.0f,-30.0f });
	CatchBallOpen->Off();
	CatchBallOpen->SetPivot({ 210.0f,-170.0f });
	MyTackleEffect->SetPivot({ 210.0f,-90.0f });
	MyTackleEffect->Off();
	MyWaterGunEffect->Off();
	MyWaterGunEffect->SetPivot({ 210.0f,-90.0f });
	MyGrowl1->SetPivot({ -120.0f,-30.0f });
	MyGrowl1->Off();
	MyGrowl1->ChangeAnimation("Growl1");
	MyGrowl2->SetPivot({ -120.0f,20.0f });
	MyGrowl2->ChangeAnimation("Growl2");
	MyGrowl2->Off();
	MyGrowl3->SetPivot({ -120.0f,70.0f });
	MyGrowl3->ChangeAnimation("Growl3");
	MyGrowl3->Off();
	EnemyGrowl1->SetPivot({ 110.0f,-90.0f });
	EnemyGrowl1->ChangeAnimation("Growl1");
	EnemyGrowl1->Off();
	EnemyGrowl2->SetPivot({ 110.0f,-140.0f });
	EnemyGrowl2->ChangeAnimation("Growl2");
	EnemyGrowl2->Off();
	EnemyGrowl3->SetPivot({ 110.0f,-180.0f });
	EnemyGrowl3->ChangeAnimation("Growl3");
	EnemyGrowl3->Off();
	EnemyScratchEffect->Off();

	EffectX = -105.0f;
	EffectY = 20.0f;
	Rock1->Off();
	Rock2->Off();
	Rock3->Off();
	Rock4->Off();
	X->Off();
	BallX = -480.0f;
	BallY = 0.0f;
	PoeCurrentPokemon_->SetAlpha(255);
	BallFall = -170.0f;
	BallFallTime = 0.0f;
	Alpha_Time = 0.0f;
}
void BattleUnitRenderer::LevelChangeEnd(GameEngineLevel* _NextLevel)
{
	BattleDataR_ = nullptr;
	{
		BattleDataR_ = nullptr;
		PlayerTime_ = 0.0f;
		PlayerTime_2 = 0.0f;
		BattleInter->SetPlayerEnd(false);
		PlayerStop = false;
		FirstMove = true;
		BallLerp = 0.0f;
		PlayerCurrentPokemon_->Off();
		if (PoeCurrentPokemon_ != nullptr)
		{
			PoeCurrentPokemon_->Death();
			PoeCurrentPokemon_ = nullptr;
		}
		if (PlayerCurrentPokemon_ != nullptr)
		{
			PlayerCurrentPokemon_->Death();
			PlayerCurrentPokemon_ = nullptr;
		}
		if (OpponentRenderer_ != nullptr)
		{
			OpponentRenderer_->Death();
			OpponentRenderer_ = nullptr;
		}
		PlayerRenderer_->Off();
		MonsterBallOP->Off();
		MonsterBallCH->Off();
		MyTackleEffect->Off();
		MyWaterGunEffect->Off();
		MyMoveTime = 0.0f;
		MyTurnEnd = true;
		EnemyTurnEnd = true;
		MyCatchEnd = true;
		SkillName_ = SkillName::None;
	}
}

void BattleUnitRenderer::DoomChit()
{
	if ((int)TimeCheck % 2 == 0)
	{
		PlayerCurrentPokemon_->SetPivot(PlayerPokemonPos_);
	}

	if ((int)TimeCheck % 2 == 1)
	{
		PlayerCurrentPokemon_->SetPivot({ -220, 71 });
	}
}

void BattleUnitRenderer::Tackle()
{
	float X = PlayerCurrentPokemon_->GetPivot().x;

	if (MyTurnEnd == false)
	{
		MyMoveTime += GameEngineTime::GetDeltaTime();

		if (MyMoveTime <= 0.3f)
		{
			PlayerCurrentPokemon_->SetPivot({ X + (MyMoveTime * 70.0f), 63.0f });
			MyTackleEffect->SetPivot({ 210.0f,-90.0f });
			MyTackleEffect->On();
		}
		if (MyMoveTime >= 0.1f)
		{
			PlayerCurrentPokemon_->SetPivot(PlayerPokemonPos_);
			//적 피격시 적 HPUI이미지 들썩
			BattleInter->GetEnemyHPUI()->SetPivot({ -450.0f,-440.0f });
			BattleInter->GetEnemyHP()->SetPivot({ -502.0f,-446.0f });
		}

		{	//적 푸키먼 피격 반짝반짝
			if (MyMoveTime >= 0.2f)
			{
				PoeCurrentPokemon_->SetAlpha(55);
				//적 HPUI이미지 아래로
				BattleInter->GetEnemyHPUI()->SetPivot({ -450.0f,-420.0f });
				BattleInter->GetEnemyHP()->SetPivot({ -502.0f,-426.0f });
			}
			if (MyMoveTime >= 0.3f)
			{
				PoeCurrentPokemon_->SetAlpha(255);
				//적 HPUI 제자리로
				BattleInter->GetEnemyHPUI()->SetPivot({ -450.0f,-430.0f });
				BattleInter->GetEnemyHP()->SetPivot({ -502.0f,-436.0f });
				MyTackleEffect->Off();
			}
			if (MyMoveTime >= 0.4f)
			{
				PoeCurrentPokemon_->SetAlpha(55);
			}
			if (MyMoveTime >= 0.5f)
			{
				PoeCurrentPokemon_->SetAlpha(255);
				MyTurnEnd = true;
			}
		}
	}
	if (MyTurnEnd == true)
	{	//적 턴도 끝나면 다시 false로 초기화 한다..?
		MyMoveTime = 0.0f;
	}
}

void BattleUnitRenderer::TailWhipMove()
{
	//꼬리흔들기 : 렌더러 회전은 그대로 무빙이 원형으로 무빙x2(자전x, 공전o)

	float4 GongJeon = PlayerCurrentPokemon_->GetPivot();
	Angle += GameEngineTime::GetDeltaTime() * 360.0f;

	if (MyTurnEnd == false)
	{
		MyMoveTime += GameEngineTime::GetDeltaTime();
		if (MyMoveTime <= 2.2f)
		{
			PlayerCurrentPokemon_->SetPivot(GongJeon.KYN_VectorRotationToDegreeZ(PlayerPokemonPos_, Angle));
		}
		if (MyMoveTime > 2.2f)
		{
			PlayerCurrentPokemon_->SetPivot(PlayerPokemonPos_);
			MyTurnEnd = true;
		}
	}
	if (MyTurnEnd == true)
	{	//적 턴도 끝나면 다시 false로 초기화 한다..?
		MyMoveTime = 0.0f;
	}
}

void BattleUnitRenderer::Opening()
{

	TimeCheck = 0.0f;
	PlayerTime_ += GameEngineTime::GetDeltaTime() * MoveSpeed;
	PlayerRenderer_->SetPivot({ 480 - PlayerTime_, 31 });

	PoeCurrentPokemon_->SetPivot({ -450 + PlayerTime_,-105 });

	if (PlayerRenderer_->GetPivot().x <= -200.0f)//float은 정확하게 딱맞아 떨어지지 않는다
	{
		PlayerRenderer_->SetPivot({ -200.0f, 31.0f });//플레이어 공식위치 고정
		PoeCurrentPokemon_->SetPivot({ 230.0f,-105.0f });//적 푸키먼 공식위치 고정
		MoveSpeed = 0.0f;
		PlayerStop = true;
		FirstMove = false;
	}

}

void BattleUnitRenderer::Opening2()
{
	//김예나 : 28일 추가 내용
	{
		float Move = PlayerRenderer_->GetPivot().x;
		float BallMoveY = MonsterBallOP->GetPivot().y;
		float BallMoveX = MonsterBallOP->GetPivot().x;

		if (BattleInter->GetPlayerEnd() == true)
		{
			PlayerRenderer_->ChangeAnimation("Go");

			if (PlayerRenderer_->GetPivot().x < -960.0f)
			{
				PlayerRenderer_->Off();
			}

			BallLerp += GameEngineTime::GetDeltaTime();
			//플레이어 무빙
			BattleUnitRenderer::PlayerRenderer_->SetPivot({ Move - (GameEngineTime::GetDeltaTime() * 900.0f),31.0f });
			if (MonsterBallOP->IsUpdate() == false)
			{
				MonsterBallOP->On();
				MonsterBallOP->ChangeAnimation("BallRoll");
			}

			{	//볼 던지기 무빙
				if (BallLerp <= 1.0f)
				{
					MonsterBallOP->SetPivot({ BallMoveX + (GameEngineTime::GetDeltaTime() * 20.0f),BallMoveY - (GameEngineTime::GetDeltaTime() * 100.0f) });
				}

				if (BallLerp > 1.0f)
				{
					MonsterBallOP->SetPivot({ BallMoveX + (GameEngineTime::GetDeltaTime() * 10.0f),BallMoveY + (GameEngineTime::GetDeltaTime() * 500.0f) });
					if (MonsterBallOP->GetPivot().y > 1000.0f)
					{
						MonsterBallOP->Off();
					}
				}

				if (BallLerp > 2.0f)
				{	//내 포켓몬 출현 + HPUI
					PlayerCurrentPokemon_->On();
					BattleInter->GetMyHPUI()->On();
					BattleInter->GetMyHP()->On();
					BattleInter->GetEXP()->On();

				}

				if (BallLerp > 3.0f && Fighting==false)
				{	//명령창 ON + 둠칫효과 시작
					
					BattleInter->GetInterfaceImage()->On();
					BattleInter->GetSelect()->On();
					DoomChit();
					Level_->OpenningEnd_ = true;
					//TailWhipMove();
					//Tackle();
					//WaterGun();
					//ShellHide();
					//EnemyRock();
					//EnemyTackle();
					//MyGrowl();
					//EnemyGrowl();
					//EnemyScratch();
					//NextPokemonAppear();
					//Catch();
					BattleInter->DoomChit();
				}
			}
		}
	}
}

void BattleUnitRenderer::WaterGun()
{
	float X = PlayerCurrentPokemon_->GetPivot().x;

	if (MyTurnEnd == false)
	{
		EffectX += GameEngineTime::GetDeltaTime() * 200.0f;
		EffectY -= GameEngineTime::GetDeltaTime() * 100.0f;
		MyMoveTime += GameEngineTime::GetDeltaTime();

		if (MyMoveTime <= 1.5f)
		{
			MyWaterGunEffect->On();
			MyWaterGunEffect->ChangeAnimation("Water");
			MyWaterGunEffect->SetPivot({ EffectX  ,EffectY});
		}
		if (MyMoveTime >= 1.6f)
		{
			MyWaterGunEffect->ChangeAnimation("WaterGun");
			PlayerCurrentPokemon_->SetPivot(PlayerPokemonPos_);
			//적 피격시 적 HPUI이미지 들썩
			BattleInter->GetEnemyHPUI()->SetPivot({ -450.0f,-440.0f });
			BattleInter->GetEnemyHP()->SetPivot({ -502.0f,-446.0f });
		}

		{	//적 푸키먼 피격 반짝반짝
			if (MyMoveTime >= 1.6f)
			{
				PoeCurrentPokemon_->SetAlpha(55);
				PoeCurrentPokemon_->SetPivot({ 220.0f,-105.0f });
				//적 HPUI이미지 아래로
				BattleInter->GetEnemyHPUI()->SetPivot({ -450.0f,-420.0f });
				BattleInter->GetEnemyHP()->SetPivot({ -502.0f,-426.0f });
			}
			if (MyMoveTime >= 1.7f)
			{
				PoeCurrentPokemon_->SetPivot({ 230.0f,-105.0f });
			}
			if (MyMoveTime >= 1.8f)
			{
				MyWaterGunEffect->Off();
				PoeCurrentPokemon_->SetPivot({ 240.0f,-105.0f });
				PoeCurrentPokemon_->SetAlpha(255);
				//적 HPUI 제자리로
				BattleInter->GetEnemyHPUI()->SetPivot({ -450.0f,-430.0f });
				BattleInter->GetEnemyHP()->SetPivot({ -502.0f,-436.0f });
			}
			if (MyMoveTime >= 2.0f)
			{
				PoeCurrentPokemon_->SetPivot({ 230.0f,-105.0f });
				PoeCurrentPokemon_->SetAlpha(55);
			
			}
			if (MyMoveTime >= 2.2f)
			{
				PoeCurrentPokemon_->SetPivot({ 230.6f,-105.0f });
				PoeCurrentPokemon_->SetAlpha(255);
				MyTurnEnd = true;
			}
		}
	}
	if (MyTurnEnd == true)
	{	//적 턴도 끝나면 다시 false로 초기화 한다..?
		MyMoveTime = 0.0f;
		MyWaterGunEffect->SetPivot({ -105.0f,20.0f });
		EffectX = -105.0f;
		EffectY = 20.0f;
	}
}

void BattleUnitRenderer::ShellHide()
{
	if (MyTurnEnd == false)
	{
		PlayerCurrentPokemon_->ChangeAnimation("ShellHide");

		//적 턴도 끝나면 다시 false로 초기화 한다..?
		if (PlayerCurrentPokemon_->IsEndAnimation() == true)
		{
			PlayerCurrentPokemon_->ChangeAnimation("Idle");
			MyTurnEnd = true;
		}
		MyMoveTime = 0.0f;
	}
}


void BattleUnitRenderer::EnemyRock()
{

	if (EnemyTurnEnd == false)
	{	//첫번째 돌
		RockSpeed = 600.0f;
		Rock1Pivot.y += (GameEngineTime::GetDeltaTime() * RockSpeed);
		Rock1->On();
		Rock1->SetPivot({ Rock1Pivot.x, Rock1Pivot.y });

	}
	if (Rock1->GetPivot().y >= 100.0f)
	{	//첫번째 돌 멈췀
		RockSpeed = 0.0f;
		Rock1->SetPivot({ -220.0f,100.0f });
		Rock1End = true;
	}
	if (Rock1End ==true)
	{	//두번째돌
		RockSpeed = 600.0f;
		Rock2Pivot.y += (GameEngineTime::GetDeltaTime() * RockSpeed);
		Rock2->On();
		Rock2->SetPivot({ Rock2Pivot.x, Rock2Pivot.y });
	}
	if (Rock2->GetPivot().y >= 100.0f)
	{	//두번째돌 멈춤
		RockSpeed = 0.0f;
		Rock2->SetPivot({ -160.0f,100.0f });
		Rock2End = true;
	}
	if (Rock2End == true)
	{	//세번째돌
		RockSpeed = 600.0f;
		Rock3Pivot.y += (GameEngineTime::GetDeltaTime() * RockSpeed);
		Rock3->On();
		Rock3->SetPivot({ Rock3Pivot.x,Rock3Pivot.y });
	}
	if (Rock3->GetPivot().y >= 100.0f)
	{	//세번째돌 멈춤
		RockSpeed = 0.0f;
		Rock3->SetPivot({ -280.0f, 100.0f });
		Rock3End = true;
	}
	if (Rock3End == true)
	{	//네번째돌
		RockSpeed = 600.0f;;
		Rock4Pivot.y += (GameEngineTime::GetDeltaTime() * RockSpeed);
		Rock4->On();
		Rock4->SetPivot({ Rock4Pivot.x,Rock4Pivot.y });
	}
	if (Rock4->GetPivot().y >= 90.0f)
	{	//네번째돌 멈춤
		RockSpeed = 0.0f;
		Rock4->SetPivot({ -240.0f, 90.0f });
		Rock4End = true;
	}
	if (Rock4End == true)
	{
		AnimationEndTime += GameEngineTime::GetDeltaTime();
		X->On();

		if (AnimationEndTime >= 1.5f)
		{
			Rock1->Off();
			Rock2->Off();
			Rock3->Off();
			Rock4->Off();
			X->Off();
			RockSpeed = 600.0f;
			Rock1End = false;
			Rock2End = false;
			Rock3End = false;
			Rock4End = false;
		}
		//피격 반짝반짝
		if (AnimationEndTime >= 1.6f)
		{	//HP UI들썪
			BattleInter->GetMyHPUI()->SetPivot({ 0.0f,-190.0f });
			BattleInter->GetMyHP()->SetPivot({ -16.0f,-200.0f });
			BattleInter->GetEXP()->SetPivot({ -80.0f,-280.0f });

			PlayerCurrentPokemon_->SetAlpha(55);
		}
		if (AnimationEndTime >= 1.7f)
		{	//HP UI 밑으로
			BattleInter->GetMyHPUI()->SetPivot({ 0.0f,-210.0f });
			BattleInter->GetMyHP()->SetPivot({ -16.0f,-220.0f });
			BattleInter->GetEXP()->SetPivot({ -80.0f,-300.0f });

			PlayerCurrentPokemon_->SetAlpha(255);
		}
		if (AnimationEndTime >= 1.8f)
		{	//HP UI 제자리로
			BattleInter->GetMyHPUI()->SetPivot({ 0.0f,-170.0f });
			BattleInter->GetMyHP()->SetPivot({ -16.0f,-180.0f });
			BattleInter->GetEXP()->SetPivot({ -80.0f,-260.0f });

			PlayerCurrentPokemon_->SetAlpha(55);
		}
		if (AnimationEndTime >= 1.9f)
		{
			PlayerCurrentPokemon_->SetAlpha(255);
			EnemyTurnEnd = true;
		}
		if (EnemyTurnEnd == true)
		{
			AnimationEndTime = 0.0f;
			//김예나 : 애니메이션 시간 초기화해주면 렌더러가 시간 관련이다보니 다시 켜지네용..다시 끄기용..
			Rock1->Off();
			Rock2->Off();
			Rock3->Off();
			Rock4->Off();
			X->Off();
			RockSpeed = 600.0f;
			Rock1End = false;
			Rock2End = false;
			Rock3End = false;
			Rock4End = false;
			Rock1Pivot = { -220.f,-100.0f };
			Rock2Pivot = { -160.f,-100.0f };
			Rock3Pivot = { -280.f,-100.0f };
			Rock4Pivot = { -240.f,-100.0f };
			Rock1->SetPivot(Rock1Pivot);
			Rock2->SetPivot(Rock2Pivot);
			Rock3->SetPivot(Rock3Pivot);
			Rock4->SetPivot(Rock4Pivot);

		}
	}
}

void BattleUnitRenderer::EnemyTackle()
{
	float X = PoeCurrentPokemon_->GetPivot().x;

	if (EnemyTurnEnd == false)
	{
		MyMoveTime += GameEngineTime::GetDeltaTime();

		if (MyMoveTime <= 0.3f)
		{
			PoeCurrentPokemon_->SetPivot({ X - (MyMoveTime * 50.0f),-105.0f });

			MyTackleEffect->SetPivot({ -200.0f,100.0f });
			MyTackleEffect->On();
		}
		if (MyMoveTime >= 0.1f)
		{
			PoeCurrentPokemon_->SetPivot({ 230.0f,-105.0f });
			//나 피격시 적 HPUI이미지 들썩
			BattleInter->GetMyHPUI()->SetPivot({ 0.0f,-190.0f });
			BattleInter->GetMyHP()->SetPivot({ -16.0f,-200.0f });
			BattleInter->GetEXP()->SetPivot({ -80.0f,-280.0f });
		}

		{	//내 푸키먼 피격 반짝반짝
			if (MyMoveTime >= 0.2f)
			{
				PlayerCurrentPokemon_ ->SetAlpha(55);
				//내 HPUI이미지 아래로
				BattleInter->GetMyHPUI()->SetPivot({ 0.0f,-210.0f });
				BattleInter->GetMyHP()->SetPivot({ -16.0f,-220.0f });
				BattleInter->GetEXP()->SetPivot({ -80.0f,-300.0f });
			}
			if (MyMoveTime >= 0.3f)
			{
				PlayerCurrentPokemon_->SetAlpha(255);
				//적 HPUI 제자리로
				BattleInter->GetMyHPUI()->SetPivot({ 0.0f,-170.0f });
				BattleInter->GetMyHP()->SetPivot({ -16.0f,-180.0f });
				BattleInter->GetEXP()->SetPivot({ -80.0f,-260.0f });
				MyTackleEffect->Off();
			}
			if (MyMoveTime >= 0.4f)
			{
				PlayerCurrentPokemon_->SetAlpha(55);
			}
			if (MyMoveTime >= 0.5f)
			{
				PlayerCurrentPokemon_->SetAlpha(255);
				EnemyTurnEnd = true;
			}
		}
	}
	if (EnemyTurnEnd == true)
	{	//적 턴도 끝나면 다시 false로 초기화 한다..?
		MyMoveTime = 0.0f;
	}
}

void BattleUnitRenderer::Catch()
{
	CatchBallTime += GameEngineTime::GetDeltaTime();
	BallX += GameEngineTime::GetDeltaTime() * 600.0f;
	BallY -= GameEngineTime::GetDeltaTime() * 150.0f;
	CatchBallPivot = { BallX,BallY };
	if (MyCatchEnd == false)
	{
		IsCatch = true;
		MonsterBallCH->SetPivot({ -480.0f,0.0f });
		MonsterBallCH->On();
		MonsterBallCH->ChangeAnimation("BallRoll");
		MonsterBallCH->SetPivot({ CatchBallPivot });
		if (CatchBallTime >= 1.1f)
		{	
			Alpha_Time += GameEngineTime::GetDeltaTime() * 200.0f;
			MonsterBallCH->Off();
			CatchBallOpen->SetPivot({ 210.0f,-170.0f });
			CatchBallOpen->On();
			CatchBallOpen->ChangeAnimation("Open");
			PoeCurrentPokemon_->SetAlpha((unsigned int)((float)255.0f - (Alpha_Time) < (float)0.0f ? (float)0.0f : (float)255.0f - (Alpha_Time)));
			//삼항연산자 알파값이 0보다 크면 해당값, 아니라면 0고정
			if (PoeCurrentPokemon_->GetAlpha() == 0)
			{
				if (IsCatch == true)
				{
					PoeCurrentPokemon_->Off();
				}
				CatchBallOpen->Off();
				BallFallTime += GameEngineTime::GetDeltaTime() * 200.0f;
				MonsterBallCH->ChangeAnimation("Ball");
				MonsterBallCH->On();
				MonsterBallCH->SetPivot({ 210.0f, BallFall + BallFallTime });
				int a = 0;
				if (MonsterBallCH->GetPivot().y >= -70.0f)
				{
					MonsterBallCH->SetPivot({ 210.0f,-70.0f });
					FallCheck = true;
				}
				if (FallCheck == true && MyCatchEnd == false)
				{
					MonsterBallCH->ChangeAnimation("Catch");
					MyCatchEnd = true;
				}
			}
		}
	}
	if (MyCatchEnd == true)
	{
		FallCheck = false;
		MonsterBallCH->SetPivot({ 210.0f,-70.0f });
	}
}

void BattleUnitRenderer::TrainerOpening1()
{
	OpponentRenderer_->On();
	PoeCurrentPokemon_->Off();

	TimeCheck = 0.0f;
	PlayerTime_ += GameEngineTime::GetDeltaTime() * MoveSpeed;
	PlayerRenderer_->SetPivot({ 480 - PlayerTime_, 31 });

	OpponentRenderer_->SetPivot({ -450 + PlayerTime_,-155 });

	if (PlayerRenderer_->GetPivot().x <= -200.0f)//float은 정확하게 딱맞아 떨어지지 않는다
	{
		PlayerRenderer_->SetPivot({ -200.0f, 31.0f });//플레이어 공식위치 고정
		OpponentRenderer_->SetPivot({ 230.0f,-155.0f });//적 트레이너 공식위치 고정
		MoveSpeed = 0.0f;
		PlayerStop = true;
		FirstMove = false;
	}
}

void BattleUnitRenderer::TrainerOpening2()
{
	//김예나 : 트레이너가 먼저 포켓몬을 꺼내고 그 다음에"Go"애니메이션이 시작됨
	{
		float Move = PlayerRenderer_->GetPivot().x;
		float BallMoveY = MonsterBallOP->GetPivot().y;
		float BallMoveX = MonsterBallOP->GetPivot().x;

		if (BattleInter->GetPlayerEnd() == true && PoeCurrentPokemon_ != nullptr)
		{
			//김예나 : 트레이너가 나가면서 푸키먼을 꺼냄
			//상대 트레이너는 포켓몬을 꺼냈다! 텍스트후

			CatchBallOpen->SetPivot({ 210.0f,-70.0f });
			CatchBallOpen->On();
			CatchBallOpen->ChangeAnimation("Open");

			MoveSpeed = 900.0f;
			PlayerTime_2 += GameEngineTime::GetDeltaTime() * MoveSpeed;

			if (IsCatch == false && Level_->GetBattleData()->GetCurrentPoePokemon()->Pokemon_->GetInfo()->GetFaint() == false)
			{
				PoeCurrentPokemon_->On();
			}
			PoeCurrentPokemon_->SetAlpha(0 + (int)PlayerTime_2);
			if (PoeCurrentPokemon_->GetAlpha() >= 255 )
			{	//알파값 255이상이면 255로 고정
				PoeCurrentPokemon_->SetAlpha(255);
				CatchBallOpen->Off();
			}
			PoeCurrentPokemon_->SetPivot({ 230.0f,-105.0f });

			OpponentRenderer_->SetPivot({ 230.0f + PlayerTime_2,-155.0f });
			//여기까지가 상대가 포켓몬을 꺼내는 애니메이션///////////////////////


			//여기부터 내가 포켓몬을 꺼내는 애니메이션//////////////////////////
			//여기서 가랏! 꼬부기! 시작

			PlayerRenderer_->ChangeAnimation("Go");

			if (OpponentRenderer_->GetPivot().x >= 1000.0f)
			{
				//트레이너가 화면밖에 나가면 끄고 Go애니메이션 실행
				OpponentRenderer_->Off();

				if (PlayerRenderer_->GetPivot().x < -960.0f)
				{
					PlayerRenderer_->Off();
				}
			}


			BallLerp += GameEngineTime::GetDeltaTime();
			//플레이어 무빙
			BattleUnitRenderer::PlayerRenderer_->SetPivot({ Move - (GameEngineTime::GetDeltaTime() * 900.0f),31.0f });
			if (MonsterBallOP->IsUpdate() == false)
			{
				MonsterBallOP->On();
				MonsterBallOP->ChangeAnimation("BallRoll");
			}

			{	//볼 던지기 무빙
				if (BallLerp <= 1.0f)
				{
					MonsterBallOP->SetPivot({ BallMoveX + (GameEngineTime::GetDeltaTime() * 20.0f),BallMoveY - (GameEngineTime::GetDeltaTime() * 100.0f) });
				}

				if (BallLerp > 1.0f)
				{
					MonsterBallOP->SetPivot({ BallMoveX + (GameEngineTime::GetDeltaTime() * 10.0f),BallMoveY + (GameEngineTime::GetDeltaTime() * 500.0f) });

					if (MonsterBallOP->GetPivot().y > 1000.0f)
					{
						MonsterBallOP->Off();
					}
				}

				if (BallLerp > 2.0f)
				{	//내 포켓몬 출현 + HPUI
					PlayerCurrentPokemon_->On();
					BattleInter->GetMyHPUI()->On();
					BattleInter->GetMyHP()->On();
					BattleInter->GetEXP()->On();

				}

				if (BallLerp > 3.0f && Fighting == false)
				{	//명령창 ON + 둠칫효과 시작

					BattleInter->GetInterfaceImage()->On();
					BattleInter->GetSelect()->On();
					DoomChit();
					Level_->OpenningEnd_ = true;
					//TailWhipMove();
					//Tackle();
					//WaterGun();
					//ShellHide();
					//EnemyRock();
					//EnemyTackle();
					//Catch();
					BattleInter->DoomChit();
				}
			}
		}
	}
}

void BattleUnitRenderer::NextPokemonAppear()
{
	//지금 포켓몬이던 다음 포켓몬이던 랜더러 변수는 PoeCurrentPokemon_로 재탕한다
	PoeCurrentPokemon_->SetAlpha(0);

	//"NPC가 다음 포켓몬을 꺼냈다!"텍스트 출력
	//BattleInter->NextNPCTalk();//문제의 함수//////////////////////////////////

	{
		if (MyTurnEnd==false)
		{
			//다음 푸키먼 등장(몬스터볼 오픈)
			IsCatch = false;
			CatchBallOpen->SetPivot({ 210.0f,-70.0f });

			CatchBallOpen->On();
			CatchBallOpen->ChangeAnimation("Open");

			MoveSpeed = 900.0f;//푸키먼 알파값 도는 속도
			PlayerTime_2 += GameEngineTime::GetDeltaTime() * MoveSpeed;

			if (IsCatch == false)
			{
				PoeCurrentPokemon_->On();
			}
			PoeCurrentPokemon_->SetAlpha(0 + (int)PlayerTime_2);
			if (PoeCurrentPokemon_->GetAlpha() >= 255)
			{	//알파값 255이상이면 255로 고정
				PoeCurrentPokemon_->SetAlpha(255);
				CatchBallOpen->Off();//몬스터볼 꺼짐
			}
			PoeCurrentPokemon_->SetPivot({ 230.0f,-105.0f });//푸키먼 위치 고정
			//여기까지가 상대가 포켓몬을 꺼내는 애니메이션///////////////////////

			BallLerp += GameEngineTime::GetDeltaTime();			

			if (BallLerp > 3.0f && Fighting == false)
			{	//명령창 ON + 둠칫효과 시작
				//이때도 텍스트출력되면서 꺼져있으니 켜지는게 맞나?
				//BattleInter->GetInterfaceImage()->On();
				//BattleInter->GetSelect()->On();
				//DoomChit();
				//BattleInter->DoomChit();
				MyTurnEnd = true;
			}
			
		}
	}
}

void BattleUnitRenderer::MyGrowl()
{
	//울음소리
	if (MyTurnEnd == false)
	{
		MyMoveTime += GameEngineTime::GetDeltaTime();
		if (MyMoveTime <= 1.5f)
		{
			MyGrowl1->On();
			MyGrowl2->On();
			MyGrowl3->On();
		}
		if (MyMoveTime > 1.5f)
		{
			MyGrowl1->Off();
			MyGrowl2->Off();
			MyGrowl3->Off();
			MyTurnEnd = true;
		}
	}
	if (MyTurnEnd == true)
	{	//적 턴도 끝나면 다시 false로 초기화 한다..?
		MyMoveTime = 0.0f;
	}
}

void BattleUnitRenderer::EnemyGrowl()
{
	//적 울음소리
	if (EnemyTurnEnd == false)
	{
		AnimationEndTime += GameEngineTime::GetDeltaTime();
		if (AnimationEndTime <= 1.5f)
		{
			EnemyGrowl1->On();
			EnemyGrowl2->On();
			EnemyGrowl3->On();
		}
		if (AnimationEndTime > 1.5f)
		{
			EnemyGrowl1->Off();
			EnemyGrowl2->Off();
			EnemyGrowl3->Off();
			EnemyTurnEnd = true;
		}
	}
	if (EnemyTurnEnd == true)
	{	//적 턴도 끝나면 다시 false로 초기화 한다..?
		AnimationEndTime = 0.0f;
	}
}

void BattleUnitRenderer::EnemyScratch()
{
	float X = PoeCurrentPokemon_->GetPivot().x;

	if (EnemyTurnEnd == false)
	{
		AnimationEndTime += GameEngineTime::GetDeltaTime();

		if (AnimationEndTime <= 0.2f)
		{
			PoeCurrentPokemon_->SetPivot({ X - (AnimationEndTime * 20.0f),-105.0f });

			EnemyScratchEffect->SetPivot({ -200.0f,100.0f });
			EnemyScratchEffect->On();
			EnemyScratchEffect->ChangeAnimation("Scratch");
		}
		if (AnimationEndTime > 0.2f)
		{
			PoeCurrentPokemon_->SetPivot({ 230.0f,-105.0f });
			//나 피격시 적 HPUI이미지 들썩
			BattleInter->GetMyHPUI()->SetPivot({ 0.0f,-190.0f });
			BattleInter->GetMyHP()->SetPivot({ -16.0f,-200.0f });
			BattleInter->GetEXP()->SetPivot({ -80.0f,-280.0f });
		}

		{	//내 푸키먼 피격 반짝반짝
			if (AnimationEndTime >= 1.0f)
			{
				PlayerCurrentPokemon_->SetAlpha(55);
				//내 HPUI이미지 아래로
				BattleInter->GetMyHPUI()->SetPivot({ 0.0f,-210.0f });
				BattleInter->GetMyHP()->SetPivot({ -16.0f,-220.0f });
				BattleInter->GetEXP()->SetPivot({ -80.0f,-300.0f });
			}
			if (AnimationEndTime >= 1.1f)
			{
				PlayerCurrentPokemon_->SetAlpha(255);
				//적 HPUI 제자리로
				BattleInter->GetMyHPUI()->SetPivot({ 0.0f,-170.0f });
				BattleInter->GetMyHP()->SetPivot({ -16.0f,-180.0f });
				BattleInter->GetEXP()->SetPivot({ -80.0f,-260.0f });
				EnemyScratchEffect->Off();
			}
			if (AnimationEndTime >= 1.2f)
			{
				PlayerCurrentPokemon_->SetAlpha(55);
			}
			if (AnimationEndTime >= 1.3f)
			{
				PlayerCurrentPokemon_->SetAlpha(255);
				EnemyTurnEnd = true;
			}
		}
	}

	if (EnemyTurnEnd == true)
	{	//적 턴도 끝나면 다시 false로 초기화 한다..?
		AnimationEndTime = 0.0f;
		EnemyScratchEffect->ChangeAnimation("ScratchStart");
	}
}