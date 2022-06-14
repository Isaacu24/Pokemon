#include "BattleUnitRenderer.h"
#include "GameEngineBase/GameEngineWindow.h"
#include "GameEngineBase/GameEngineTime.h"
#include <GameEngine/GameEngineImageManager.h>
#include "BattleLevel.h"
#include <tchar.h>
#include <GameEngine/GameEngine.h>

#include "BattleInterface.h"

GameEngineRenderer* BattleUnitRenderer::PlayerRenderer_ = nullptr;

BattleUnitRenderer::BattleUnitRenderer()
	: PlayerCurrentPokemon_(nullptr)
	, PoeCurrentPokemon_(nullptr)
	, OpponentRenderer_(nullptr)//������ ����
	, FirstMove(true)
	, PlayerPokemonPos_({ -220, 63 })
	, OpponentPokemonPos_({ -450, -105 })//200,-105
	, PlayerRendererPos_({ 480,31 })
	, OpponentRenderer_Pos_()
	, MoveSpeed(900.0f)//������200
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
	, IsCatch(false)
	, MonsterBallCH(nullptr)
{
}	
BattleUnitRenderer::~BattleUnitRenderer() 
{
}

void BattleUnitRenderer::Start()
{
	Level_ = dynamic_cast<BattleLevel*>(GetLevel());

	SetPosition({ GameEngineWindow::GetScale().Half() });

	//������ ����

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
		default:
			break;
		}

		if (SkillName_ != SkillName::None && (MyTurnEnd == true && EnemyTurnEnd == true && MyCatchEnd == true))
		{
			SkillName_ = SkillName::None;
			Level_->DoingSkillAnimation_ = false;
		}
	}


}

void BattleUnitRenderer::Render()
{
	ShowDebugValue();
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


void BattleUnitRenderer::LevelChangeStart(GameEngineLevel* _PrevLevel)
{

	if (BattleInter == nullptr)
	{
		BattleInter = dynamic_cast<BattleInterface*>(GetLevel()->FindActor("BattleInterface"));
	}
	FirstMove = true;
	Fighting = false;
	MoveSpeed = 900.0f;//������200
	RockSpeed = 300.0f;
	CatchBallTime = 0.0f;
	IsCatch = false;

	BattleDataR_ = Level_->GetBattleData();
	if (nullptr == PlayerCurrentPokemon_)
	{
		//ǪŰ��
		PlayerCurrentPokemon_ = CreateRenderer(BattleDataR_->GetCurrentPlayerPokemon()->GetPokemon()->GetInfo()->GetMyBattleBack()
			, 3, RenderPivot::CENTER, PlayerPokemonPos_);

	}
	if (PoeCurrentPokemon_ == nullptr)
	{
		PoeCurrentPokemon_ = CreateRenderer(BattleDataR_->GetCurrentPoePokemon()->GetPokemon()->GetInfo()->GetMyBattleFront()
			, 3, RenderPivot::CENTER, OpponentPokemonPos_);
	}

	if (PlayerRenderer_ == nullptr)
	{
		//�÷��̾�
		PlayerRenderer_ = CreateRenderer("Player.bmp", 4, RenderPivot::CENTER, PlayerRendererPos_);
		PlayerRenderer_->CreateAnimation("Player.bmp", "Stop", 0, 0, 0.1f, false);
		PlayerRenderer_->CreateAnimation("PlayerAnimation.bmp", "Go", 0, 4, 0.1f, false);

		//��� Ʈ���̳�
		//�迹�� : �ӽ÷� ���̹� �̹����� �־���� ��Ȳ�� ���� �߻� ���ϸ�ó�� �ٸ� ������ �߰� �ϸ� �ɰŰ��ƿ�


		//��
		MonsterBallOP = CreateRenderer("MonsterBall4.bmp", 4);
		MonsterBallOP->CreateAnimation("BallRoll.bmp", "BallRoll", 0, 5, 0.05f, true);
		MonsterBallOP->CreateAnimation("BallRoll.bmp", "Ball", 0, 0, 0.05f, false);
		MonsterBallOP->CreateAnimation("Catch4.bmp", "Catch", 0, 3, 0.2f, false);

		MonsterBallCH = CreateRenderer("MonsterBall4.bmp", 4);
		MonsterBallCH->CreateAnimation("BallRoll.bmp", "BallRoll", 0, 5, 0.05f, true);
		MonsterBallCH->CreateAnimation("BallRoll.bmp", "Ball", 0, 0, 0.05f, false);
		MonsterBallCH->CreateAnimation("Catch4.bmp", "Catch", 0, 3, 0.2f, false);
		// �� �׳� ���°ɷ� �ߴµ� �ʹݿ� �ȵ��°� �ϰ������ ���� �÷��̾� ó�� ���� �����ʿ�

		CatchBallOpen = CreateRenderer("MonsterBall_Open4.bmp", 4);
		CatchBallOpen->CreateAnimation("MonsterBall_Open4.bmp", "Open", 0, 1, 0.3f, false);

		MyWaterGunEffect = CreateRenderer("WaterGun4.bmp", 4);
		MyWaterGunEffect->CreateAnimation("WaterGun4.bmp", "WaterGun", 0, 2, 0.1f, false);
		MyWaterGunEffect->CreateAnimation("WaterGun4.bmp", "Water", 0, 0, 0.1f, false);
		MyTackleEffect = CreateRenderer("Tackle4.bmp", 4);

		PlayerCurrentPokemon_->CreateAnimation("SquirtleB.bmp", "Idle", 0, 0, 0.0f, false);
		PlayerCurrentPokemon_->CreateAnimation("ShellHide.bmp", "ShellHide", 0, 7, 0.1f, false);

		Rock2 = CreateRenderer("Rock4.bmp", 4);
		Rock3 = CreateRenderer("Rock4.bmp", 4);
		Rock4 = CreateRenderer("Rock4.bmp", 2);//ǪŰ�� �ڷ� �Ѿ�� ��
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

	if (BattleDataR_->IsWild() == true) // ���⼭ �߻��������� �����ϴ°�
	{

	}
	else if(OpponentRenderer_ == nullptr)
	{
		if (BattleDataR_->GetNameString() == "Green")
		{
			OpponentRenderer_ = CreateRenderer("Rival_Battle.bmp", 4, RenderPivot::CENTER, OpponentPokemonPos_);
			OpponentRenderer_->Off();
			OpponentRenderer_->SetPivot({ -450.0f,-155.0f });
		}
		else if (BattleDataR_->GetNameString() == "Brock")
		{
			OpponentRenderer_ = CreateRenderer("Ung_Battle.bmp", 4, RenderPivot::CENTER, OpponentPokemonPos_);
			OpponentRenderer_->Off();
			OpponentRenderer_->SetPivot({ -450.0f,-155.0f });
		}
	
	}
	
	// �庴�� : ó�� �ѹ��� ����� Start���� �ϴ°� �� ���ƺ��̴µ� PlayerCurrentPokemon_ �ø��� �������ܼ� �ϴ� �����
	// ���Ŀ� �����ʿ��ҵ�



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
			PlayerCurrentPokemon_->SetPivot({ X + (MyMoveTime * 900.0f),63.0f });
			MyTackleEffect->SetPivot({ 210.0f,-90.0f });
			MyTackleEffect->On();
		}
		if (MyMoveTime >= 0.1f)
		{
			PlayerCurrentPokemon_->SetPivot(PlayerPokemonPos_);
			//�� �ǰݽ� �� HPUI�̹��� ���
			BattleInter->GetEnemyHPUI()->SetPivot({ -450.0f,-440.0f });
			BattleInter->GetEnemyHP()->SetPivot({ -502.0f,-446.0f });
		}

		{	//�� ǪŰ�� �ǰ� ��¦��¦
			if (MyMoveTime >= 0.2f)
			{
				PoeCurrentPokemon_->SetAlpha(55);
				//�� HPUI�̹��� �Ʒ���
				BattleInter->GetEnemyHPUI()->SetPivot({ -450.0f,-420.0f });
				BattleInter->GetEnemyHP()->SetPivot({ -502.0f,-426.0f });
			}
			if (MyMoveTime >= 0.3f)
			{
				PoeCurrentPokemon_->SetAlpha(255);
				//�� HPUI ���ڸ���
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
	{	//�� �ϵ� ������ �ٽ� false�� �ʱ�ȭ �Ѵ�..?
		MyMoveTime = 0.0f;
	}
}

void BattleUnitRenderer::TailWhipMove()
{
	//�������� : ������ ȸ���� �״�� ������ �������� ����x2(����x, ����o)

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
	{	//�� �ϵ� ������ �ٽ� false�� �ʱ�ȭ �Ѵ�..?
		MyMoveTime = 0.0f;
	}
}

void BattleUnitRenderer::Opening()
{

	TimeCheck = 0.0f;
	PlayerTime_ += GameEngineTime::GetDeltaTime() * MoveSpeed;
	PlayerRenderer_->SetPivot({ 480 - PlayerTime_, 31 });

	PoeCurrentPokemon_->SetPivot({ -450 + PlayerTime_,-105 });

	if (PlayerRenderer_->GetPivot().x <= -200.0f)//float�� ��Ȯ�ϰ� ���¾� �������� �ʴ´�
	{
		PlayerRenderer_->SetPivot({ -200.0f, 31.0f });//�÷��̾� ������ġ ����
		PoeCurrentPokemon_->SetPivot({ 230.0f,-105.0f });//�� ǪŰ�� ������ġ ����
		MoveSpeed = 0.0f;
		PlayerStop = true;
		FirstMove = false;
	}

}

void BattleUnitRenderer::Opening2()
{
	//�迹�� : 28�� �߰� ����
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
			//�÷��̾� ����
			BattleUnitRenderer::PlayerRenderer_->SetPivot({ Move - (GameEngineTime::GetDeltaTime() * 900.0f),31.0f });
			if (MonsterBallOP->IsUpdate() == false)
			{
				MonsterBallOP->On();
				MonsterBallOP->ChangeAnimation("BallRoll");
			}

			{	//�� ������ ����
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
				{	//�� ���ϸ� ���� + HPUI
					PlayerCurrentPokemon_->On();
					BattleInter->GetMyHPUI()->On();
					BattleInter->GetMyHP()->On();
					BattleInter->GetEXP()->On();

				}

				if (BallLerp > 3.0f && Fighting==false)
				{	//���â ON + ��ĩȿ�� ����
					
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
					NextPokemonAppear();
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
	EffectX += GameEngineTime::GetDeltaTime() * 200.0f;
	EffectY -= GameEngineTime::GetDeltaTime() * 100.0f;

	if (MyTurnEnd == false)
	{
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
			//�� �ǰݽ� �� HPUI�̹��� ���
			BattleInter->GetEnemyHPUI()->SetPivot({ -450.0f,-440.0f });
			BattleInter->GetEnemyHP()->SetPivot({ -502.0f,-446.0f });
		}

		{	//�� ǪŰ�� �ǰ� ��¦��¦
			if (MyMoveTime >= 1.6f)
			{
				PoeCurrentPokemon_->SetAlpha(55);
				PoeCurrentPokemon_->SetPivot({ 220.0f,-105.0f });
				//�� HPUI�̹��� �Ʒ���
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
				//�� HPUI ���ڸ���
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
	{	//�� �ϵ� ������ �ٽ� false�� �ʱ�ȭ �Ѵ�..?
		MyMoveTime = 0.0f;
	}
}

void BattleUnitRenderer::ShellHide()
{
	if (MyTurnEnd == false)
	{
		PlayerCurrentPokemon_->ChangeAnimation("ShellHide");
		MyTurnEnd = true;
	}
	if (MyTurnEnd == true)
	{	//�� �ϵ� ������ �ٽ� false�� �ʱ�ȭ �Ѵ�..?
		if (PlayerCurrentPokemon_->IsEndAnimation() == true)
		{
			PlayerCurrentPokemon_->ChangeAnimation("Idle");
		}
		MyMoveTime = 0.0f;
	}
}


void BattleUnitRenderer::EnemyRock()
{

	if (EnemyTurnEnd == false)
	{	//ù��° ��
		RockSpeed = 600.0f;
		Rock1Pivot.y += (GameEngineTime::GetDeltaTime() * RockSpeed);
		Rock1->On();
		Rock1->SetPivot({ Rock1Pivot.x, Rock1Pivot.y });

	}
	if (Rock1->GetPivot().y >= 100.0f)
	{	//ù��° �� �ح�
		RockSpeed = 0.0f;
		Rock1->SetPivot({ -220.0f,100.0f });
		Rock1End = true;
	}
	if (Rock1End ==true)
	{	//�ι�°��
		RockSpeed = 600.0f;
		Rock2Pivot.y += (GameEngineTime::GetDeltaTime() * RockSpeed);
		Rock2->On();
		Rock2->SetPivot({ Rock2Pivot.x, Rock2Pivot.y });
	}
	if (Rock2->GetPivot().y >= 100.0f)
	{	//�ι�°�� ����
		RockSpeed = 0.0f;
		Rock2->SetPivot({ -160.0f,100.0f });
		Rock2End = true;
	}
	if (Rock2End == true)
	{	//����°��
		RockSpeed = 600.0f;
		Rock3Pivot.y += (GameEngineTime::GetDeltaTime() * RockSpeed);
		Rock3->On();
		Rock3->SetPivot({ Rock3Pivot.x,Rock3Pivot.y });
	}
	if (Rock3->GetPivot().y >= 100.0f)
	{	//����°�� ����
		RockSpeed = 0.0f;
		Rock3->SetPivot({ -280.0f, 100.0f });
		Rock3End = true;
	}
	if (Rock3End == true)
	{	//�׹�°��
		RockSpeed = 600.0f;;
		Rock4Pivot.y += (GameEngineTime::GetDeltaTime() * RockSpeed);
		Rock4->On();
		Rock4->SetPivot({ Rock4Pivot.x,Rock4Pivot.y });
	}
	if (Rock4->GetPivot().y >= 90.0f)
	{	//�׹�°�� ����
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
		//�ǰ� ��¦��¦
		if (AnimationEndTime >= 1.6f)
		{
			BattleInter->GetMyHPUI()->SetPivot({ 0.0f,-200.0f });

			PlayerCurrentPokemon_->SetAlpha(55);
		}
		if (AnimationEndTime >= 1.7f)
		{
			BattleInter->GetMyHPUI()->SetPivot({ 0.0f,-170.0f });
			PlayerCurrentPokemon_->SetAlpha(255);
		}
		if (AnimationEndTime >= 1.8f)
		{
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
			//�迹�� : �ִϸ��̼� �ð� �ʱ�ȭ���ָ� �������� �ð� �����̴ٺ��� �ٽ� �����׿�..�ٽ� �����..
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
			//�� �ǰݽ� �� HPUI�̹��� ���
			BattleInter->GetMyHPUI()->SetPivot({ 0.0f,-190.0f });
			BattleInter->GetMyHP()->SetPivot({ 0.0f,-200.0f });
			BattleInter->GetEXP()->SetPivot({ -80.0f,-280.0f });
		}

		{	//�� ǪŰ�� �ǰ� ��¦��¦
			if (MyMoveTime >= 0.2f)
			{
				PlayerCurrentPokemon_ ->SetAlpha(55);
				//�� HPUI�̹��� �Ʒ���
				BattleInter->GetMyHPUI()->SetPivot({ 0.0f,-210.0f });
				BattleInter->GetMyHP()->SetPivot({ 0.0f,-220.0f });
				BattleInter->GetEXP()->SetPivot({ -80.0f,-300.0f });
			}
			if (MyMoveTime >= 0.3f)
			{
				PlayerCurrentPokemon_->SetAlpha(255);
				//�� HPUI ���ڸ���
				BattleInter->GetMyHPUI()->SetPivot({ 0.0f,-170.0f });
				BattleInter->GetMyHP()->SetPivot({ 0.0f,-180.0f });
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
	{	//�� �ϵ� ������ �ٽ� false�� �ʱ�ȭ �Ѵ�..?
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
			//���׿����� ���İ��� 0���� ũ�� �ش簪, �ƴ϶�� 0����
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

	if (PlayerRenderer_->GetPivot().x <= -200.0f)//float�� ��Ȯ�ϰ� ���¾� �������� �ʴ´�
	{
		PlayerRenderer_->SetPivot({ -200.0f, 31.0f });//�÷��̾� ������ġ ����
		OpponentRenderer_->SetPivot({ 230.0f,-155.0f });//�� Ʈ���̳� ������ġ ����
		MoveSpeed = 0.0f;
		PlayerStop = true;
		FirstMove = false;
	}
}

void BattleUnitRenderer::TrainerOpening2()
{
	//�迹�� : Ʈ���̳ʰ� ���� ���ϸ��� ������ �� ������"Go"�ִϸ��̼��� ���۵�
	{
		float Move = PlayerRenderer_->GetPivot().x;
		float BallMoveY = MonsterBallOP->GetPivot().y;
		float BallMoveX = MonsterBallOP->GetPivot().x;

		if (BattleInter->GetPlayerEnd() == true)
		{
			//�迹�� : Ʈ���̳ʰ� �����鼭 ǪŰ���� ����
			//��� Ʈ���̳ʴ� ���ϸ��� ���´�! �ؽ�Ʈ��

			CatchBallOpen->SetPivot({ 210.0f,-70.0f });
			CatchBallOpen->On();
			CatchBallOpen->ChangeAnimation("Open");

			MoveSpeed = 900.0f;
			PlayerTime_2 += GameEngineTime::GetDeltaTime() * MoveSpeed;

			if (IsCatch == false)
			{
				PoeCurrentPokemon_->On();
			}
			PoeCurrentPokemon_->SetAlpha(0 + (int)PlayerTime_2);
			if (PoeCurrentPokemon_->GetAlpha() >= 255)
			{	//���İ� 255�̻��̸� 255�� ����
				PoeCurrentPokemon_->SetAlpha(255);
				CatchBallOpen->Off();
			}
			PoeCurrentPokemon_->SetPivot({ 230.0f,-105.0f });

			OpponentRenderer_->SetPivot({ 230.0f + PlayerTime_2,-155.0f });
			//��������� ��밡 ���ϸ��� ������ �ִϸ��̼�///////////////////////


			//������� ���� ���ϸ��� ������ �ִϸ��̼�//////////////////////////
			//���⼭ ����! ���α�! ����

			PlayerRenderer_->ChangeAnimation("Go");

			if (OpponentRenderer_->GetPivot().x >= 1000.0f)
			{
				//Ʈ���̳ʰ� ȭ��ۿ� ������ ���� Go�ִϸ��̼� ����
				OpponentRenderer_->Off();

				if (PlayerRenderer_->GetPivot().x < -960.0f)
				{
					PlayerRenderer_->Off();
				}
			}


			BallLerp += GameEngineTime::GetDeltaTime();
			//�÷��̾� ����
			BattleUnitRenderer::PlayerRenderer_->SetPivot({ Move - (GameEngineTime::GetDeltaTime() * 900.0f),31.0f });
			if (MonsterBallOP->IsUpdate() == false)
			{
				MonsterBallOP->On();
				MonsterBallOP->ChangeAnimation("BallRoll");
			}

			{	//�� ������ ����
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
				{	//�� ���ϸ� ���� + HPUI
					PlayerCurrentPokemon_->On();
					BattleInter->GetMyHPUI()->On();
					BattleInter->GetMyHP()->On();
					BattleInter->GetEXP()->On();

				}

				if (BallLerp > 3.0f && Fighting == false)
				{	//���â ON + ��ĩȿ�� ����

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
	//���� ���ϸ��̴� ���� ���ϸ��̴� ������ ������ PoeCurrentPokemon_�� �����Ѵ�
	PoeCurrentPokemon_->Off();
	PoeCurrentPokemon_->SetAlpha(0);

	//"NPC�� ���� ���ϸ��� ���´�!"�ؽ�Ʈ ���
	//BattleInter->NextNPCTalk();

	{
		if (/*���� ������ �ɷ����� NextBattleCheck����bool�ϳ� �־���ҵ�*/true)
		{
			//���� ǪŰ�� ����(���ͺ� ����)

			CatchBallOpen->SetPivot({ 210.0f,-70.0f });
			CatchBallOpen->On();
			CatchBallOpen->ChangeAnimation("Open");

			MoveSpeed = 900.0f;//ǪŰ�� ���İ� ���� �ӵ�
			PlayerTime_2 += GameEngineTime::GetDeltaTime() * MoveSpeed;

			if (IsCatch == false)
			{
				PoeCurrentPokemon_->On();
			}
			PoeCurrentPokemon_->SetAlpha(0 + (int)PlayerTime_2);
			if (PoeCurrentPokemon_->GetAlpha() >= 255)
			{	//���İ� 255�̻��̸� 255�� ����
				PoeCurrentPokemon_->SetAlpha(255);
				CatchBallOpen->Off();//���ͺ� ����
			}
			PoeCurrentPokemon_->SetPivot({ 230.0f,-105.0f });//ǪŰ�� ��ġ ����
			//��������� ��밡 ���ϸ��� ������ �ִϸ��̼�///////////////////////

			BallLerp += GameEngineTime::GetDeltaTime();			

			if (BallLerp > 3.0f && Fighting == false)
			{	//���â ON + ��ĩȿ�� ����

				BattleInter->GetInterfaceImage()->On();
				BattleInter->GetSelect()->On();
				DoomChit();
				Level_->OpenningEnd_ = true;
				BattleInter->DoomChit();
			}
			
		}
	}
}