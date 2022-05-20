#include <GameEngineBase/GameEngineInput.h>
#include <GameEngine/GameEngine.h>

#include <GameEngineContentsCore/GameEngineContentFont.h>

#include "BattleLevel.h"
#include "BattleUnitRenderer.h"
#include "BattleBackground.h"
#include "BattleEngine.h"



BattleLevel::BattleLevel()
	: Interface(nullptr)
	, BState_(BattleState::Openning)
	, OpenningEnd_(false)
	, EnddingEnd_(false)
	, PlayerRed_(nullptr)
	, BEngine_(nullptr)
	, OneTalk(false)
{

}

BattleLevel::~BattleLevel()
{

}

void BattleLevel::Loading()
{
	CreateActor<BattleBackground>();
	BEngine_ = CreateActor<BattleEngine>();

	Interface = CreateActor<BattleInerface>(3);
	Interface->SetPosition({ 720.0f, 548.0f });
	
	//�迹��:�׽�Ʈ �ڵ�
	PlayerStopCheck = CreateActor<BattleUnitRenderer>();
	Fonts = CreateActor<GameEngineContentFont>(3);
	Fonts->SetPosition({ 50, 485 });
}

void BattleLevel::Update()
{
	switch (BState_)
	{
	case BattleState::Openning:
		if (OpenningEnd_ == true)
		{
			BState_ = BattleState::Selecet;
		}
		return;
		break;
	case BattleState::Selecet:
		Interface->MoveKey();
		break;
	case BattleState::Battle:
		return;
		break;
	case BattleState::Endding:
		if (EnddingEnd_ == true)
		{
			// ���� �̵�
			return;
		}
		break;
	}

	if (PlayerStopCheck->GetPlayerStop()==true&&OneTalk==false)
	{
		//�迹��:�÷��̾� ����� ��Ʈ��� �׽�Ʈ
		Fonts->ShowString("Wild Bulbarsaur\\is appear!!", false);
		OneTalk = true;
		//�� ������ �߰� ��Ʈ�� "���� ���α�!" ����� ���α� ���� + ��ƲĿ�ǵ� �̶� ����
	}
} 


void BattleLevel::LevelChangeStart(GameEngineLevel * _PrevLevel)
{
	if (PlayerRed_ == nullptr)
	{
		PlayerRed_  = PlayerRed::MainRed_;
	}
	//BState_ = BattleState::Openning
	BState_ = BattleState::Selecet;
	OpenningEnd_ = false;
	EnddingEnd_ = false;
	ShowOpenning();
}

void BattleLevel::ShowOpenning()
{

}


void BattleLevel::LevelChangeEnd(GameEngineLevel* _NextLevel)
{
	OpenningEnd_ = false;
	EnddingEnd_ = false;
}

void BattleLevel::ShowEndding()
{

}