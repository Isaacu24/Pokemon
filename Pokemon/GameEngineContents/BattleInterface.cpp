#include <GameEngineBase/GameEngineInput.h>
#include <GameEngineBase/GameEngineTime.h>
#include "BattleInterface.h"
#include "BattleLevel.h"
#include "PokemonEngine.h"
#include "BattleEngine.h"
#include <GameEngineContentsCore/GameEngineContentFont.h>
#include "BattleUnitRenderer.h"
#include <GameEngineBase/GameEngineMath.h>
#include <GameEngine/GameEngineImageManager.h>

BattleInterface::BattleInterface()
	:TimeCheck(0.0f)
	, CurOrder(BattleOrder::None)
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
	, BattleUnit(nullptr)
	, OneTalk(false)
	, Fonts(nullptr)
	, PlayerEnd(false)
	, EmptyString_(false)
	
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
	GameEngineInput::GetInst()->CreateKey("StartBattlePage", 'H');
	//

	InterfaceImage = CreateRenderer("Battle_Select.bmp",2);
	InterfaceImage->Off();
	Select = CreateRenderer("Select.bmp", 5);
	Select->Off();

	EnemyHPUI = CreateRenderer("EnemyHPBackground4.bmp",2);
	EnemyHPUI->Off();
	EnemyHP = CreateRenderer("EnemyHPBar4.bmp", 3);
	EnemyHP->Off();
	MyHPUI = CreateRenderer("FriendlyHPBackground4.bmp",2);
	MyHPUI->Off();
	MyHP = CreateRenderer("FriendlyHPBar4.bmp", 3);
	MyHP->Off();
	EXP = CreateRenderer("FriendlyHPExp4.bmp", 3);
	EXP->Off();
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

	//������ ���� RegistActor=�̹� ������� Actor�� Level�� ���
	GetLevel()->RegistActor("BattleInterface", this);
	//�迹��:�׽�Ʈ �ڵ�
	BattleUnit = Level_->CreateActor<BattleUnitRenderer>();
	Fonts = Level_->CreateActor<GameEngineContentFont>(3);
	Fonts->SetPosition({ 50, 485 });
}

void BattleInterface::Render()
{
}

void BattleInterface::Update()
{
	//MoveKey();
	TimeCheck += (GameEngineTime::GetDeltaTime() * 2.0f);
	if (Level_->GetBattleState() != BattleState::BattlePage)
	{
		SelectOrder();
		OrderCheck();
	}

	if (BattleUnit->GetPlayerStop() == true && OneTalk == false)
	{
		//�迹��:�÷��̾� ����� ��Ʈ��� �׽�Ʈ
		Fonts->ShowString("Wild NAME\\is appear!!\\Go!!\\NAME!!", false);
		OneTalk = true;
		//�� ������ �߰� ��Ʈ�� "���� ���α�!" ����� ���α� ���� + ��ƲĿ�ǵ� �̶� ����
	}

	{
		// ��Ʈ ����� �Ϸ�ǰ� Ű�Է� ���
		if (Fonts->IsWait())
		{
			// Z Ű �Է½� ���� ���� ���
			if (GameEngineInput::GetInst()->IsDown("Z") == true)
			{
				EnemyHPUI->On();
				EnemyHP->On();
				// ���� ���� ��� �Լ�
				Fonts->NextString();
			}
		}
		// ���� ������ ���� �� == ������ ��
		if (Fonts->IsEnd())
		{
			// ��� ��ȭ�� ������ �� z Ű������ 
			if (GameEngineInput::GetInst()->IsDown("Z") == true)
			{
				PlayerEnd = true;
				Fonts->EndFont();
			}
		}
	}

}

bool BattleInterface::BattleKey()
{
	if (EmptyString_ == false)
	{
		if (GameEngineInput::GetInst()->IsDown("SSelect"))
		{
			if (DownFont_->IsEnd())
			{
				DownFont_->EndFont();
				return true;
			}
			else
			{
				DownFont_->SkipCharAnimation();
				return false;
			}
		}
	}
	else
	{
		EmptyString_ = false;
		return true;
	}
	return false;
}

void BattleInterface::ShowUsedSkillString(const std::string& _AttPokemon, const std::string& _AttSkill)
{
		DownFont_->EndFont();
		DownFont_->ShowString(_AttPokemon + " Used\\" + _AttSkill + "!");
}

void BattleInterface::ShowPoeFaintString(const std::string& _PoePokemon)
{
	DownFont_->EndFont();
	DownFont_->ShowString("Poe " + _PoePokemon + "\\fainted!");
}

void BattleInterface::ShowSupperEffectString()
{
	DownFont_->EndFont();
	DownFont_->ShowString("It's Supe\\effective!");
}

void BattleInterface::ShowCriticalHitString()
{
	DownFont_->EndFont();
	DownFont_->ShowString("A critical hit!");
}

void BattleInterface::ShowNotEffective()
{
	DownFont_->EndFont();
	DownFont_->ShowString("It's not Very\\effective;");
}

void BattleInterface::ShowFailed()
{
	DownFont_->EndFont();
	DownFont_->ShowString("But it failed!");
}

void BattleInterface::ShowRankUpAndDown(const std::string& _Pokemon, PokemonAbility _Ability, int _Rank)
{
	DownFont_->EndFont();
	std::string Rankfont = _Pokemon;
	Rankfont += " is\\";
	Rankfont += AbilityString(_Ability);
	Rankfont += RankString(_Rank);
	DownFont_->ShowString(Rankfont);
}

std::string BattleInterface::AbilityString(PokemonAbility _Ability)
{
	switch (_Ability)
	{
	case PokemonAbility::Att:
		return "ATTACK";
			break;
	case PokemonAbility::Def:
		return "DEFENSE";
		break;
	case PokemonAbility::SpAtt:
		return "SPECIAL ATTACK";
		break;
	case PokemonAbility::SpDef:
		return "SPECIAL DEFENSE";
		break;
	case PokemonAbility::Speed:
		return "SPEED";
		break;
	case PokemonAbility::Accuracy:
		return "ACCURACY";
		break;
	case PokemonAbility::Evasion:
		return "EVASION";
		break;
	default:
		break;
	}
}

std::string BattleInterface::RankString(int _Rank)
{
	switch (_Rank)
	{
	case 1:
		return " rose!";
		break;
	case 2:
		return " sharply rose!";
		break;
	case 3:
		return " rose drastically!";
		break;
	case -1:
		return " fell!";
		break;
	case -2:
		return " harshly fell!";
		break;
	case -3:
		return " severely fell!";
		break;
	}
}



bool BattleInterface::MoveKey()
{
	if (InterfaceImage->IsUpdate()==true)
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
	}


	// ������ : Debug
	if (GameEngineInput::GetInst()->IsDown("StartBattlePage"))
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
	if (InterfaceImage->IsUpdate() == true)
	{
		if ((GetSelect()->GetPivot().x == -190.0f && GetSelect()->GetPivot().y == -25.0f) && true == GameEngineInput::GetInst()->IsDown("SSelect"))
		{	//�ο�� ����
			CurOrder = BattleOrder::Fight;
			GetSelect()->SetPivot({-680.0f,-30.0f});//���1�� ��ġ
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
			GetSelect()->SetPivot({ -190.0f,-25.0f });
			CurOrder = BattleOrder::None;
		}
	}
}