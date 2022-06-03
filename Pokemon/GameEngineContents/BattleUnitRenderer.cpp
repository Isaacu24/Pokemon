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
	, PlayerPokemonPos_({ -220, 43 })
	, OpponentPokemonPos_({ -450, -105 })//200,-105
	, PlayerRendererPos_({ 480,31 })
	, OpponentRenderer_Pos_()
	, MoveSpeed(900.0f)//������200
	, PlayerStop(false)
	, TimeCheck(0.0f)
	, MonsterBall(nullptr)
	, BattleDataR_(nullptr)
	, Level_(nullptr)
	, MyMoveTime(0.0f)
	, MyTurnEnd(false)
	, Angle(0.0f)
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
}

void BattleUnitRenderer::Update()
{
	TimeCheck += (GameEngineTime::GetDeltaTime() * 2.0f);
	if (FirstMove == true)
	{
		TimeCheck = 0.0f;
		PlayerTime_ += GameEngineTime::GetDeltaTime() * MoveSpeed;
		PlayerRenderer_->SetPivot({ 480 - PlayerTime_, 31 });

		PoeCurrentPokemon_->SetPivot({ -450 + PlayerTime_,-105 });

		if (PlayerRenderer_->GetPivot().x <= -200.0f)//float�� ��Ȯ�ϰ� ���¾� �������� �ʴ´�
		{
			MoveSpeed = 0.0f;
			PlayerStop = true;
			FirstMove = false;
		}
	}

	//�迹�� : 28�� �߰� ����
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
			//�÷��̾� ����
			BattleUnitRenderer::PlayerRenderer_->SetPivot({ Move - (GameEngineTime::GetDeltaTime() * 900.0f),31.0f });
			if (MonsterBall->IsUpdate() == false)
			{
				MonsterBall->On();
				MonsterBall->ChangeAnimation("BallRoll");
			}

			{	//�� ������ ����
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
				{	//�� ���ϸ� ���� + HPUI
					PlayerCurrentPokemon_->On();
					BattleInter->GetMyHPUI()->On();
					BattleInter->GetMyHP()->On();
					BattleInter->GetEXP()->On();
				}

				if (BallLerp > 3.0f)
				{	//���â ON + ��ĩȿ�� ����
					BattleInter->GetInterfaceImage()->On();
					BattleInter->GetSelect()->On();
					DoomChit();
					TailWhipMove();
					//Tackle();
					BattleInter->DoomChit();
				}
			}
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
	MoveSpeed = 900.0f;//������200

	BattleDataR_ = Level_->GetBattleData();
	
	// �庴�� : ó�� �ѹ��� ����� Start���� �ϴ°� �� ���ƺ��̴µ� PlayerCurrentPokemon_ �ø��� �������ܼ� �ϴ� �����
	// ���Ŀ� �����ʿ��ҵ�
	if (nullptr == PlayerRenderer_)
	{
		//�÷��̾�
		PlayerRenderer_ = CreateRenderer("Player.bmp", 4, RenderPivot::CENTER, PlayerRendererPos_);
		PlayerRenderer_->CreateAnimation("Player.bmp", "Stop", 0, 0, 0.1f, false);
		PlayerRenderer_->CreateAnimation("PlayerAnimation.bmp", "Go", 0, 4, 0.1f, false);

		//ǪŰ��
		PlayerCurrentPokemon_ = CreateRenderer(BattleDataR_->GetCurrentPlayerPokemon()->GetPokemon()->GetInfo()->GetMyBattleBack()
			, 3, RenderPivot::CENTER, PlayerPokemonPos_);
		PoeCurrentPokemon_ = CreateRenderer(BattleDataR_->GetCurrentPoePokemon()->GetPokemon()->GetInfo()->GetMyBattleFront()
			, 3, RenderPivot::CENTER, OpponentPokemonPos_);

		//��
		MonsterBall = CreateRenderer("MonsterBall4.bmp", 0);
		MonsterBall->CreateAnimation("BallRoll.bmp", "BallRoll", 0, 5, 0.05f, true);
		// �� �׳� ���°ɷ� �ߴµ� �ʹݿ� �ȵ��°� �ϰ������ ���� �÷��̾� ó�� ���� �����ʿ�
	}

	PlayerRenderer_->On();
	PlayerRenderer_->ChangeAnimation("Stop");
	PlayerCurrentPokemon_->Off();
	PlayerCurrentPokemon_->SetPivot(PlayerPokemonPos_);
	PoeCurrentPokemon_->On();
	MonsterBall->Off();
	MonsterBall->SetPivot({ -220.0f,-30.0f });
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
		PlayerCurrentPokemon_->SetPivot({ -220, 61 });
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
			PlayerCurrentPokemon_->SetPivot({ X + (MyMoveTime * 900.0f),43.0f });
		}
		if (MyMoveTime >= 0.1f)
		{
			PlayerCurrentPokemon_->SetPivot(PlayerPokemonPos_);
			//�� �ǰݽ� �� HPUI�̹��� ���
			BattleInter->GetEnemyHPUI()->SetPivot({ -450.0f,-440.0f });
		}

		{	//�� ǪŰ�� �ǰ� ��¦��¦
			if (MyMoveTime >= 0.2f)
			{
				PoeCurrentPokemon_->SetAlpha(55);
				//�� HPUI�̹��� ���ڸ���
				BattleInter->GetEnemyHPUI()->SetPivot({ -450.0f,-420.0f });
			}
			if (MyMoveTime >= 0.3f)
			{
				PoeCurrentPokemon_->SetAlpha(255);
				BattleInter->GetEnemyHPUI()->SetPivot({ -450.0f,-430.0f });
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
