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
	, OpponentRenderer_(nullptr)//병문씨 도움
	, FirstMove(true)
	, PlayerPokemonPos_({ -220, 63 })
	, OpponentPokemonPos_({ -450, -105 })//200,-105
	, PlayerRendererPos_({ 480,31 })
	, OpponentRenderer_Pos_()
	, MoveSpeed(900.0f)//원래는200
	, PlayerStop(false)
	, TimeCheck(0.0f)
	, MonsterBall(nullptr)
	, MyTackleEffect(nullptr)
	, BattleDataR_(nullptr)
	, Level_(nullptr)
	, MyMoveTime(0.0f)
	, MyTurnEnd(false)
	, Angle(0.0f)
	, Fighting(false)
	, MyWaterGunEffect(nullptr)
	, BallLerp(0.0f)
	, BattleInter(nullptr)
	, EnemyTackleEffect(nullptr)
	, PlayerTime_(0.0f)
	, EffectX(-105.0f)
	, EffectY(20.0f)
{
}

BattleUnitRenderer::~BattleUnitRenderer() 
{
}

void BattleUnitRenderer::Start()
{
	Level_ = dynamic_cast<BattleLevel*>(GetLevel());

	SetPosition({ GameEngineWindow::GetScale().Half() });

	//동원씨 도움
	BattleInter = dynamic_cast<BattleInterface*>(GetLevel()->FindActor("BattleInterface"));

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
}

void BattleUnitRenderer::Update()
{
	TimeCheck += (GameEngineTime::GetDeltaTime() * 2.0f);
	Opening();
	Opening2();
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


	// Poe Hp
	{
		char Text[10] = { 0 };
		sprintf_s(Text, "HP : %d", PoePokemon->GetInfo()->GetHp());
		TextOut(GameEngine::BackBufferDC(), PoeCurrentPokemon_->GetSortingPivot().ix()
			, PoeCurrentPokemon_->GetSortingPivot().iy(), Text, static_cast<int>(strlen(Text)));
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
}


void BattleUnitRenderer::LevelChangeStart(GameEngineLevel* _PrevLevel)
{
	FirstMove = true;
	Fighting = false;
	MoveSpeed = 900.0f;//원래는200

	BattleDataR_ = Level_->GetBattleData();
	
	// 장병문 : 처음 한번만 만들기 Start에서 하는게 더 좋아보이는데 PlayerCurrentPokemon_ 올리면 문제생겨서 일단 여기둠
	// 추후에 수정필요할듯
	if (nullptr == PlayerRenderer_)
	{
		//플레이어
		PlayerRenderer_ = CreateRenderer("Player.bmp", 4, RenderPivot::CENTER, PlayerRendererPos_);
		PlayerRenderer_->CreateAnimation("Player.bmp", "Stop", 0, 0, 0.1f, false);
		PlayerRenderer_->CreateAnimation("PlayerAnimation.bmp", "Go", 0, 4, 0.1f, false);

		//푸키먼
		PlayerCurrentPokemon_ = CreateRenderer(BattleDataR_->GetCurrentPlayerPokemon()->GetPokemon()->GetInfo()->GetMyBattleBack()
			, 3, RenderPivot::CENTER, PlayerPokemonPos_);
		PoeCurrentPokemon_ = CreateRenderer(BattleDataR_->GetCurrentPoePokemon()->GetPokemon()->GetInfo()->GetMyBattleFront()
			, 3, RenderPivot::CENTER, OpponentPokemonPos_);

		//볼
		MonsterBall = CreateRenderer("MonsterBall4.bmp", 0);
		MonsterBall->CreateAnimation("BallRoll.bmp", "BallRoll", 0, 5, 0.05f, true);
		// 볼 그냥 도는걸로 했는데 초반에 안도는거 하고싶으면 위에 플레이어 처럼 따로 생성필요

		MyWaterGunEffect = CreateRenderer("WaterGun4.bmp", 4);
		MyWaterGunEffect->CreateAnimation("WaterGun4.bmp", "WaterGun", 0, 2, 0.1f, false);
		MyWaterGunEffect->CreateAnimation("WaterGun4.bmp", "Water", 0, 0, 0.1f, false);
		MyTackleEffect = CreateRenderer("Tackle4.bmp",4);
	}

	PlayerRenderer_->On();
	PlayerRenderer_->ChangeAnimation("Stop");
	PlayerCurrentPokemon_->Off();
	PlayerCurrentPokemon_->SetPivot(PlayerPokemonPos_);
	PoeCurrentPokemon_->On();
	MonsterBall->Off();
	MonsterBall->SetPivot({ -220.0f,-30.0f });
	MyTackleEffect->SetPivot({ 210.0f,-90.0f });
	MyTackleEffect->Off();
	MyWaterGunEffect->Off();
	MyWaterGunEffect->SetPivot({ 210.0f,-90.0f });
	EffectX = -105.0f;
	EffectY = 20.0f;
}
void BattleUnitRenderer::LevelChangeEnd(GameEngineLevel* _NextLevel)
{
	BattleDataR_ = nullptr;
	{
		BattleDataR_ = nullptr;
		PlayerTime_ = 0.0f;
		BattleInter->SetPlayerEnd(false);
		PlayerStop = false;
		FirstMove = true;
		BallLerp = 0.0f;
		PlayerCurrentPokemon_->Off();
		PoeCurrentPokemon_->Off();
		PlayerRenderer_->Off();
		MonsterBall->Off();
		MyTackleEffect->Off();
		MyWaterGunEffect->Off();
		MyMoveTime = 0.0f;
		MyTurnEnd = false;
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

			MyTackleEffect->On();
		}
		if (MyMoveTime >= 0.1f)
		{
			PlayerCurrentPokemon_->SetPivot(PlayerPokemonPos_);
			//적 피격시 적 HPUI이미지 들썩
			BattleInter->GetEnemyHPUI()->SetPivot({ -450.0f,-440.0f });
		}

		{	//적 푸키먼 피격 반짝반짝
			if (MyMoveTime >= 0.2f)
			{
				PoeCurrentPokemon_->SetAlpha(55);
				//적 HPUI이미지 아래로
				BattleInter->GetEnemyHPUI()->SetPivot({ -450.0f,-420.0f });
			}
			if (MyMoveTime >= 0.3f)
			{
				PoeCurrentPokemon_->SetAlpha(255);
				//적 HPUI 제자리로
				BattleInter->GetEnemyHPUI()->SetPivot({ -450.0f,-430.0f });
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
	if (FirstMove == true)
	{
		TimeCheck = 0.0f;
		PlayerTime_ += GameEngineTime::GetDeltaTime() * MoveSpeed;
		PlayerRenderer_->SetPivot({ 480 - PlayerTime_, 31 });

		PoeCurrentPokemon_->SetPivot({ -450 + PlayerTime_,-105 });

		if (PlayerRenderer_->GetPivot().x <= -200.0f)//float은 정확하게 딱맞아 떨어지지 않는다
		{
			MoveSpeed = 0.0f;
			PlayerStop = true;
			FirstMove = false;
		}
	}
}

void BattleUnitRenderer::Opening2()
{
	//김예나 : 28일 추가 내용
	{
		float Move = PlayerRenderer_->GetPivot().x;
		float BallMoveY = MonsterBall->GetPivot().y;
		float BallMoveX = MonsterBall->GetPivot().x;

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
			if (MonsterBall->IsUpdate() == false)
			{
				MonsterBall->On();
				MonsterBall->ChangeAnimation("BallRoll");
			}

			{	//볼 던지기 무빙
				if (BallLerp <= 1.0f)
				{
					MonsterBall->SetPivot({ BallMoveX + (GameEngineTime::GetDeltaTime() * 20.0f),BallMoveY - (GameEngineTime::GetDeltaTime() * 100.0f) });
				}

				if (BallLerp > 1.0f)
				{
					MonsterBall->SetPivot({ BallMoveX + (GameEngineTime::GetDeltaTime() * 10.0f),BallMoveY + (GameEngineTime::GetDeltaTime() * 500.0f) });
					if (MonsterBall->GetPivot().y > 1000.0f)
					{
						MonsterBall->Off();
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
					//TailWhipMove();
					//Tackle();
					WaterGun();
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
			//적 피격시 적 HPUI이미지 들썩
			BattleInter->GetEnemyHPUI()->SetPivot({ -450.0f,-440.0f });
		}

		{	//적 푸키먼 피격 반짝반짝
			if (MyMoveTime >= 1.6f)
			{
				PoeCurrentPokemon_->SetAlpha(55);
				PoeCurrentPokemon_->SetPivot({ 220.0f,-105.0f });
				//적 HPUI이미지 아래로
				BattleInter->GetEnemyHPUI()->SetPivot({ -450.0f,-420.0f });
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
	}
}