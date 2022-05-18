#include <GameEngineBase/GameEngineInput.h>

#include "BattleLevel.h"
#include "BattleBackground.h"
#include "BattleInerface.h"


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
	
	CreateActor<BattleBackground>();
	
	BattleInerface* Interface = CreateActor<BattleInerface>(3);
	Interface->SetPosition({ 720.0f, 548.0f });

	Select = CreateActor<BattleSelect>(4);
	Select->SetPosition({ 530.0f,522.0f }); //�⺻ Fight��ġ ����
}

void BattleLevel::Update()
{
	if ((Select->GetPosition().x == 530.0f && Select->GetPosition().y == 522.0f) &&true == GameEngineInput::GetInst()->IsDown("SDown"))
	{	//Fight���� Pokemon����
		Select->SetPosition({ 530.0f,585.0f });
	}

	if ((Select->GetPosition().x == 530.0f && Select->GetPosition().y == 585.0f) && true == GameEngineInput::GetInst()->IsDown("SUp"))
	{	//Pokemon���� Fight��
		Select->SetPosition({ 530.0f,522.0f });
	}

	if ((Select->GetPosition().x == 530.0f && Select->GetPosition().y == 522.0f) && true == GameEngineInput::GetInst()->IsDown("SRight"))
	{	//Fight���� Bag����
		Select->SetPosition({ 750.0f,522.0f });
	}

	if ((Select->GetPosition().x == 750.0f && Select->GetPosition().y == 522.0f) && true == GameEngineInput::GetInst()->IsDown("SLeft"))
	{	//Bag���� Fight��
		Select->SetPosition({ 530.0f,522.0f });
	}

	if ((Select->GetPosition().x == 750.0f && Select->GetPosition().y == 522.0f) && true == GameEngineInput::GetInst()->IsDown("SDown"))
	{	//Bag���� Run����
		Select->SetPosition({ 750.0f,585.0f });
	}

	if ((Select->GetPosition().x == 750.0f && Select->GetPosition().y == 585.0f) && true == GameEngineInput::GetInst()->IsDown("SUp"))
	{	//Run���� Bag����
		Select->SetPosition({ 750.0f,522.0f });
	}

	if ((Select->GetPosition().x == 750.0f && Select->GetPosition().y == 585.0f) && true == GameEngineInput::GetInst()->IsDown("SLeft"))
	{	//Run���� Pokemon����
		Select->SetPosition({ 530.0f,585.0f });
	}

	if ((Select->GetPosition().x == 530.0f && Select->GetPosition().y == 585.0f) && true == GameEngineInput::GetInst()->IsDown("SRight"))
	{	//Pokemon���� Run����
		Select->SetPosition({ 750.0f,585.0f });
	}
} 

