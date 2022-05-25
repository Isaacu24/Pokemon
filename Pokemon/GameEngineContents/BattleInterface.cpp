#include <GameEngineBase/GameEngineInput.h>
#include <GameEngineBase/GameEngineTime.h>
#include "BattleInterface.h"
#include "BattleLevel.h"
#include "PokemonEngine.h"
#include "BattleEngine.h"
#include <GameEngineContentsCore/GameEngineContentFont.h>
#include "BattleUnitRenderer.h"

BattleInterface::BattleInterface()
	:TimeCheck(0.0f)
	,CurOrder(BattleOrder::None)
	, InterfaceImage(nullptr)
	, Select(nullptr)
	, MyHPUI(nullptr)
	, EnemyHPUI(nullptr)
	, MyHP(nullptr)
	, EnemyHP(nullptr)
	, EXP(nullptr)
	, BattleCommend(nullptr)
	, MainInterface(nullptr)
	, DownFont_(nullptr)
	, Level_(nullptr)
	, PlayerStopCheck(nullptr)
	, OneTalk(false)
	, Fonts(nullptr)
	, PlayerEnd(false)
{

}

BattleInterface::~BattleInterface()
{

}

void BattleInterface::Start()
{
	// ���� ����
	Level_ = dynamic_cast<BattleLevel*>(GetLevel());
	// ��Ʈ ����
	DownFont_ = Level_->CreateActor<GameEngineContentFont>(10);
	DownFont_->SetPosition({ 50, 485 });
	// Ű ����
	GameEngineInput::GetInst()->CreateKey("SLeft", VK_LEFT);
	GameEngineInput::GetInst()->CreateKey("SRight", VK_RIGHT);
	GameEngineInput::GetInst()->CreateKey("SDown", VK_DOWN);
	GameEngineInput::GetInst()->CreateKey("SUp", VK_UP);
	GameEngineInput::GetInst()->CreateKey("SSelect", 'Z');
	GameEngineInput::GetInst()->CreateKey("SCancel", 'X');
	//

	InterfaceImage = CreateRenderer("Battle_Select.bmp",2);
	
	Select = CreateRenderer("Select.bmp", 3);

	MyHPUI = CreateRenderer("FriendlyHPBackground4.bmp",2);
	EnemyHPUI = CreateRenderer("EnemyHPBackground4.bmp",2);
	MyHP = CreateRenderer("FriendlyHPBar4.bmp", 3);
	EnemyHP = CreateRenderer("EnemyHPBar4.bmp", 3);
	EXP = CreateRenderer("FriendlyHPExp4.bmp", 3);
	BattleCommend = CreateRenderer("BattleCommend4.bmp", 4);
	//Player = CreateRenderer("Player.bmp",0);

	//=========������ ��ġ ����==========//
	Select->SetPivot({ -190.0f, -25.0f });

	MyHPUI->SetPivot({ 0.0f,-170.0f });
	EnemyHPUI->SetPivot({ -450.0f,-430.0f });
	EnemyHP->SetPivot({ -406.0f,-430.0f });
	MyHP->SetPivot({ 80.0f, -170.0f });
	EXP->SetPivot({48.0f,-170.0f});
	BattleCommend->SetPivot({-240.0f,0.0f});
	BattleCommend->Off();//��ƲĿ�ǵ�� Fight�����϶���
	//Player->SetPivot({ -450.0f,-180.0f });//������ġ

	//�迹��:�׽�Ʈ �ڵ�
	PlayerStopCheck = Level_->CreateActor<BattleUnitRenderer>();
	Fonts = Level_->CreateActor<GameEngineContentFont>(3);
	Fonts->SetPosition({ 50, 485 });
}

void BattleInterface::Render()
{
}

void BattleInterface::Update()
{
	//MoveKey();

	DoomChit();
	TimeCheck += (GameEngineTime::GetDeltaTime() * 2.0f);
	if (Level_->GetBattleState() != BattleState::BattlePage)
	{
		SelectOrder();
		OrderCheck();
	}

	if (PlayerStopCheck->GetPlayerStop() == true && OneTalk == false)
	{
		//�迹��:�÷��̾� ����� ��Ʈ��� �׽�Ʈ
		Fonts->ShowString("Wild Bulbarsaur\\is appear!!\\Go!!\\Charizard!!", false);
		OneTalk = true;
		//�� ������ �߰� ��Ʈ�� "���� ���α�!" ����� ���α� ���� + ��ƲĿ�ǵ� �̶� ����
	}

	{
		float Move = BattleUnitRenderer::PlayerRenderer_->GetPivot().x;
		//���� �÷��̾� ���� ��ġ(x)�� ������

		// ��Ʈ ����� �Ϸ�ǰ� Ű�Է� ���
		if (Fonts->IsWait())
		{
			// Z Ű �Է½� ���� ���� ���
			if (GameEngineInput::GetInst()->IsDown("Z") == true)
			{
				// ���� ���� ��� �Լ�
				Fonts->NextString();
			}
		}
		// ���� ������ ���� �� == ������ ��
		if (Fonts->IsEnd())
		{
			PlayerEnd = true;
			// ��� ��ȭ�� ������ �� z Ű������ 
			if (GameEngineInput::GetInst()->IsDown("Z") == true)
			{
				//�ִϸ��̼�ü����
				BattleUnitRenderer::PlayerRenderer_->ChangeAnimation("Go");
				//�̶� �÷��̾ �������� ��������

				if (BattleUnitRenderer::PlayerRenderer_->GetPivot().x < -960.0f)
				{
					BattleUnitRenderer::PlayerRenderer_->Off();
				}
			}
			// ��� ��ȭ�� ������ �� x Ű������ ����
			else if (GameEngineInput::GetInst()->IsDown("X") == true)
			{
				Fonts->EndFont();
			}
		}

		if (PlayerEnd == true)
		{
			BattleUnitRenderer::PlayerRenderer_->SetPivot({Move-(GameEngineTime::GetDeltaTime()*300.0f),31.0f});
			//�÷��̾� ���� ����
		}
	}

}

bool BattleInterface::BattleKey()
{
	if (GameEngineInput::GetInst()->IsDown("SSelect"))
	{
		return true;
	}
	return false;
}

void BattleInterface::UsedSkillString(const std::string& _AttPokemon, const std::string& _AttSkill)
{
	DownFont_->EndFont();
	DownFont_->ShowString(_AttPokemon + " Used\\"+ _AttSkill + "!");
}

void BattleInterface::PoeFaintString(const std::string& _PoePokemon)
{
	DownFont_->EndFont();
	DownFont_->ShowString("Poe " + _PoePokemon + "\\fainted!");
}

void BattleInterface::SupperEffectString()
{
	DownFont_->EndFont();
	DownFont_->ShowString("It's Super effective!");
}

void BattleInterface::CriticalHitString()
{
	DownFont_->EndFont();
	DownFont_->ShowString("A critical hit!");
}

void BattleInterface::NotEffective()
{
	DownFont_->EndFont();
	DownFont_->ShowString("It's not Very effective;");
}

bool BattleInterface::MoveKey()
{
	if ((GetSelect()->GetPivot().x == -190.0f && GetSelect()->GetPivot().y == -25.0f) && true == GameEngineInput::GetInst()->IsDown("SDown"))
	{	//Fight���� Pokemon����
		GetSelect()->SetPivot({ -190.0f,35.0f });
	}

	if ((GetSelect()->GetPivot().x == -190.0f && GetSelect()->GetPivot().y == 35.0f) && true == GameEngineInput::GetInst()->IsDown("SUp"))
	{	//Pokemon���� Fight��
		GetSelect()->SetPivot({ -190.0f,-25.0f });
	}

	if ((GetSelect()->GetPivot().x == -190.0f && GetSelect()->GetPivot().y == -25.0f) && true == GameEngineInput::GetInst()->IsDown("SRight"))
	{	//Fight���� Bag����
		GetSelect()->SetPivot({ 30.0f,-25.0f });
	}

	if ((GetSelect()->GetPivot().x == 30.0f && GetSelect()->GetPivot().y == -25.0f) && true == GameEngineInput::GetInst()->IsDown("SLeft"))
	{	//Bag���� Fight��
		GetSelect()->SetPivot({ -190.0f,-25.0f });
	}

	if ((GetSelect()->GetPivot().x == 30.0f && GetSelect()->GetPivot().y == -25.0f) && true == GameEngineInput::GetInst()->IsDown("SDown"))
	{	//Bag���� Run����
		GetSelect()->SetPivot({ 30.0f,35.0f });
	}

	if ((GetSelect()->GetPivot().x == 30.0f && GetSelect()->GetPivot().y == 35.0f) && true == GameEngineInput::GetInst()->IsDown("SUp"))
	{	//Run���� Bag����
		GetSelect()->SetPivot({ 30.0f,-25.0f });
	}

	if ((GetSelect()->GetPivot().x == -190.0f && GetSelect()->GetPivot().y == 35.0f) && true == GameEngineInput::GetInst()->IsDown("SRight"))
	{	//Pokemon���� Run����
		GetSelect()->SetPivot({ 30.0f,35.0f });
	}

	if ((GetSelect()->GetPivot().x == 30.0f && GetSelect()->GetPivot().y == 35.0f) && true == GameEngineInput::GetInst()->IsDown("SLeft"))
	{	//Run���� Pokemon����
		GetSelect()->SetPivot({ -190.0f,35.0f });
	}



	// ������ : Debug
	if (GameEngineInput::GetInst()->IsDown("SSelect"))
	{
		return true;
	}
	return false;
	//
}

void BattleInterface::DoomChit()
{
	if ((int)TimeCheck % 2 == 0)
	{
		MyHPUI->SetPivot({ 0.0f,-174.0f });
		MyHP->SetPivot({ 80.0f, -174.0f });
		EXP->SetPivot({ 48.0f,-174.0f });
	}

	if ((int)TimeCheck % 2 == 1)
	{
		MyHPUI->SetPivot({ 0.0f,-170.0f });
		MyHP->SetPivot({ 80.0f, -170.0f });
		EXP->SetPivot({ 48.0f,-170.0f });
	}
}

void BattleInterface::OrderCheck()
{
	switch (CurOrder)
	{
	case BattleOrder::Fight:
		BattleCommend->On();
		break;
	case BattleOrder::Pokemon:
		break;
	case BattleOrder::Bag:
		break;
	case BattleOrder::Run:
		break;
	default:
		break;
	}
}

void BattleInterface::SelectOrder()
{
	if ((GetSelect()->GetPivot().x == -190.0f && GetSelect()->GetPivot().y == -25.0f) && true == GameEngineInput::GetInst()->IsDown("SSelect"))
	{	//�ο�� ����
		CurOrder = BattleOrder::Fight;
	}

	if ((GetSelect()->GetPivot().x == -190.0f && GetSelect()->GetPivot().y == 35.0f) && true == GameEngineInput::GetInst()->IsDown("SSelect"))
	{	//���ϸ� ����
		CurOrder = BattleOrder::Pokemon;
	}

	if ((GetSelect()->GetPivot().x == 30.0f && GetSelect()->GetPivot().y == -25.0f) && true == GameEngineInput::GetInst()->IsDown("SSelect"))
	{	//���� ����
		CurOrder = BattleOrder::Bag;
	}
	
	if ((GetSelect()->GetPivot().x == 30.0f && GetSelect()->GetPivot().y == 35.0f) && true == GameEngineInput::GetInst()->IsDown("SSelect"))
	{
		CurOrder = BattleOrder::Run;
	}

	if (CurOrder == BattleOrder::Fight && true == GameEngineInput::GetInst()->IsDown("SCancel"))
	{
		BattleCommend->Off();
		CurOrder = BattleOrder::None;
	}
}