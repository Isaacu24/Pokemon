#include "BattleInerface.h"
#include <GameEngineBase/GameEngineInput.h>
#include <GameEngineBase/GameEngineTime.h>


BattleInerface::BattleInerface()
	:TimeCheck(0.0f)
{

}

BattleInerface::~BattleInerface()
{

}

void BattleInerface::Start()
{
	GameEngineInput::GetInst()->CreateKey("SLeft", VK_LEFT);
	GameEngineInput::GetInst()->CreateKey("SRight", VK_RIGHT);
	GameEngineInput::GetInst()->CreateKey("SDown", VK_DOWN);
	GameEngineInput::GetInst()->CreateKey("SUp", VK_UP);
	GameEngineInput::GetInst()->CreateKey("SSelect", 'A');


	InterfaceImage = CreateRenderer("Battle_Select.bmp",0);
	
	Select = CreateRenderer("Select.bmp", 1);

	MyHPUI = CreateRenderer("FriendlyHPBackground4.bmp",1);
	EnemyHPUI = CreateRenderer("EnemyHPBackground4.bmp",1);
	MyHP = CreateRenderer("FriendlyHPBar4.bmp", 2);
	EnemyHP = CreateRenderer("EnemyHPBar4.bmp", 2);
	EXP = CreateRenderer("FriendlyHPExp4.bmp", 2);

	//=========������ ��ġ ����==========//
	Select->SetPivot({ -190.0f, -25.0f });

	MyHPUI->SetPivot({ 0.0f,-170.0f });
	EnemyHPUI->SetPivot({ -450.0f,-430.0f });
	EnemyHP->SetPivot({ -406.0f,-430.0f });
	MyHP->SetPivot({ 80.0f, -170.0f });
	EXP->SetPivot({48.0f,-170.0f});
}

void BattleInerface::Render()
{
}

void BattleInerface::Update()
{
	//MoveKey();
	DoomChit();
	TimeCheck += GameEngineTime::GetDeltaTime();
}


void BattleInerface::MoveKey()
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

void BattleInerface::DoomChit()
{
	if ((int)TimeCheck % 2 == 0)
	{
		MyHPUI->SetPivot({ 0.0f,-174.0f });
		EnemyHPUI->SetPivot({ -450.0f,-434.0f });
		EnemyHP->SetPivot({ -406.0f,-434.0f });
		MyHP->SetPivot({ 80.0f, -174.0f });
		EXP->SetPivot({ 48.0f,-174.0f });
	}

	if ((int)TimeCheck % 2 == 1)
	{
		MyHPUI->SetPivot({ 0.0f,-170.0f });
		EnemyHPUI->SetPivot({ -450.0f,-430.0f });
		EnemyHP->SetPivot({ -406.0f,-430.0f });
		MyHP->SetPivot({ 80.0f, -170.0f });
		EXP->SetPivot({ 48.0f,-170.0f });
	}
}