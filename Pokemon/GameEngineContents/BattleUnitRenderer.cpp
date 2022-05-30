#include "BattleUnitRenderer.h"
#include "GameEngineBase/GameEngineWindow.h"
#include "GameEngineBase/GameEngineTime.h"
#include <GameEngine/GameEngineImageManager.h>

#include "BattleInterface.h"

GameEngineRenderer* BattleUnitRenderer::PlayerRenderer_ = nullptr;

BattleUnitRenderer::BattleUnitRenderer() 
	: PlayerCurrentPokemon_(nullptr)
	, PoeCurrentPokemon_(nullptr)
	, OpponentRenderer_(nullptr)
	, FirstMove(true)
	, PlayerPokemonPos_({ -220, 63 })
	, OpponentPokemonPos_({ -450, -105 })//200,-105
	, PlayerRendererPos_({ 480,31 })
	, OpponentRenderer_Pos_()
	, MoveSpeed(200.0f)
	, PlayerStop(false)
	, TimeCheck(0.0f)
	, MonsterBall(nullptr)
{
}

BattleUnitRenderer::~BattleUnitRenderer() 
{
}

void BattleUnitRenderer::Start()
{
	SetPosition({ GameEngineWindow::GetScale().Half() });
	PlayerCurrentPokemon_ = CreateRenderer("SquirtleB.bmp", 3, RenderPivot::CENTER, PlayerPokemonPos_);
	PlayerCurrentPokemon_->Off();

	PoeCurrentPokemon_ = CreateRenderer("BulbasaurF.bmp", 3, RenderPivot::CENTER, OpponentPokemonPos_);

	PlayerRenderer_ = CreateRenderer("Player.bmp", 4, RenderPivot::CENTER, PlayerRendererPos_);
	
	GameEngineImage* Image = GameEngineImageManager::GetInst()->Find("PlayerAnimation.bmp");
	Image->CutCount(5, 1);

	PlayerRenderer_->CreateAnimation("PlayerAnimation.bmp", "Go", 0, 4, 0.1f, false);

	MonsterBall = CreateRenderer("MonsterBall4.bmp", 0);
	MonsterBall->Off();
	MonsterBall->SetPivot({ -220.0f,-30.0f });
	GameEngineImage* Image1 = GameEngineImageManager::GetInst()->Find("BallRoll.bmp");
	Image1->CutCount(6, 1);
	MonsterBall->CreateAnimation("BallRoll.bmp", "BallRoll", 0, 5, 0.05f, true);

	//������ ����
	BattleInter = dynamic_cast<BattleInterface*>(GetLevel()->FindActor("BattleInterface"));
}

void BattleUnitRenderer::Update()
{
	TimeCheck += (GameEngineTime::GetDeltaTime() * 2.0f);
	if (FirstMove == true)
	{
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
					BattleInter->DoomChit();
				}
			}
		}
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