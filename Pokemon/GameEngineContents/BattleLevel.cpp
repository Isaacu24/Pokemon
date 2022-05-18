#include <GameEngineBase/GameEngineInput.h>
#include <GameEngine/GameEngine.h>

#include "BattleLevel.h"
#include "BattleBackground.h"



BattleLevel::BattleLevel()
{

}

BattleLevel::~BattleLevel()
{

}

void BattleLevel::Loading()
{
	GameEngineInput::GetInst()->CreateKey("SLeft", VK_LEFT);
	GameEngineInput::GetInst()->CreateKey("SRight", VK_RIGHT);
	GameEngineInput::GetInst()->CreateKey("SDown", VK_DOWN);
	GameEngineInput::GetInst()->CreateKey("SUp", VK_UP);
	GameEngineInput::GetInst()->CreateKey("SSelect", 'A');
	
	CreateActor<BattleBackground>();
	
	Interface = CreateActor<BattleInerface>(3);
	Interface->SetPosition({ 720.0f, 548.0f });

	Interface->Select->SetPivot({ -190.0f, -25.0f });

}

void BattleLevel::Update()
{
	if ((Interface->Select->GetPivot().x == -190.0f && Interface->Select->GetPivot().y == -25.0f) &&true == GameEngineInput::GetInst()->IsDown("SDown"))
	{	//Fight���� Pokemon����
		Interface->Select->SetPivot({ -190.0f,35.0f });
	}

	if ((Interface->Select->GetPivot().x == -190.0f && Interface->Select->GetPivot().y == 35.0f) && true == GameEngineInput::GetInst()->IsDown("SUp"))
	{	//Pokemon���� Fight��
		Interface->Select->SetPivot({ -190.0f,-25.0f });
	}

	if ((Interface->Select->GetPivot().x == -190.0f && Interface->Select->GetPivot().y == -25.0f) && true == GameEngineInput::GetInst()->IsDown("SRight"))
	{	//Fight���� Bag����
		Interface->Select->SetPivot({ 30.0f,-25.0f });
	}

	if ((Interface->Select->GetPivot().x == 30.0f && Interface->Select->GetPivot().y == -25.0f) && true == GameEngineInput::GetInst()->IsDown("SLeft"))
	{	//Bag���� Fight��
		Interface->Select->SetPivot({ -190.0f,-25.0f });
	}

	if ((Interface->Select->GetPivot().x == 30.0f && Interface->Select->GetPivot().y == -25.0f) && true == GameEngineInput::GetInst()->IsDown("SDown"))
	{	//Bag���� Run����
		Interface->Select->SetPivot({ 30.0f,35.0f });
	}

	if ((Interface->Select->GetPivot().x == 30.0f && Interface->Select->GetPivot().y == 35.0f) && true == GameEngineInput::GetInst()->IsDown("SUp"))
	{	//Run���� Bag����
		Interface->Select->SetPivot({ 30.0f,-25.0f });
	}

	if ((Interface->Select->GetPivot().x == -190.0f && Interface->Select->GetPivot().y == 35.0f) && true == GameEngineInput::GetInst()->IsDown("SRight"))
	{	//Pokemon���� Run����
		Interface->Select->SetPivot({ 30.0f,35.0f });
	}

	if ((Interface->Select->GetPivot().x == 30.0f && Interface->Select->GetPivot().y == 35.0f) && true == GameEngineInput::GetInst()->IsDown("SLeft"))
	{	//Run���� Pokemon����
		Interface->Select->SetPivot({ -190.0f,35.0f });
	}

} 

