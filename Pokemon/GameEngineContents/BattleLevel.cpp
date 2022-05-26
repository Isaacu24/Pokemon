#include <GameEngineBase/GameEngineInput.h>
#include <GameEngine/GameEngine.h>

#include <GameEngineContentsCore/GameEngineContentFont.h>
#include "BattleTestNPC1.h"

#include "BattleLevel.h"
#include "BattleUnitRenderer.h"
#include "BattleBackground.h"
#include "BattleEngine.h"
#include "BattleNPCInterface.h"
#include "WildPokemonNPC.h"
#include "PokemonInfoManager.h"



BattleLevel::BattleLevel()
	: Interface_(nullptr)
	, BState_(BattleState::Openning)
	, OpenningEnd_(false)
	, EnddingEnd_(false)
	// �����
	, PlayerCurrentPokemon_(nullptr)
	, PoeCurrentPokemon_(nullptr)
	, PlayerRed_(nullptr)
	, Opponent_(nullptr)
	, PlayerStopCheck(nullptr)
	, OneTalk(false)
	, Fonts(nullptr)
	, BattleData_(nullptr)
	, BattleManager_(nullptr)
{

}

BattleLevel::~BattleLevel()
{	
	if (BattleData_ != nullptr)
	{
		{
			delete BattleData_;
		}
	}
	if (Opponent_ != nullptr)
	{
		delete Opponent_->GetPokemonList()[0];
		delete PlayerRed_->GetPokemonList().front();
	}
}

void BattleLevel::Loading()
{

	/// ������ : BattleTestNPC1 DebugTest
	//Pokemon* Debug = CreateActor<Pokemon>();
	//Debug->SetInfo("Squirtle");
	//Opponent_ = CreateActor<BattleTestNPC1>();
	//Opponent_->PushPokemon(Debug);
	//PlayerCurrentPokemon_ = CreateActor<Pokemon>();
	//PlayerCurrentPokemon_->SetInfo("Charmander");
	//PoeCurrentPokemon_ = Opponent_->GetPokemonList();
	// Debug



	CreateActor<BattleBackground>();

	Interface_ = CreateActor<BattleInterface>(3);
	Interface_->SetPosition({ 720.0f, 548.0f });

}

void BattleLevel::Update()
{
	switch (BState_)
	{
	case BattleState::Openning:
		if (OpenningEnd_ == true)
		{
			BState_ = BattleState::SelecetPage;
		}
		return;
		break;
	case BattleState::SelecetPage:
		if (Interface_->MoveKey() == true)
		{
			StartBattlePage("Tackle", "Scratch");
		}
		break;
	case BattleState::BattlePage:
		if (Interface_->BattleKey())
		{
			BattleManager_->Update();
		}
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
} 

void BattleLevel::StartBattlePage(const std::string& _PlayerSkill, const std::string& _PoeSkill)
{
	RefreshPokemon();
	{
		bool Bool = false;
		std::vector<PokemonSkill*>& PlayerSkill = PlayerCurrentPokemon_->GetPokemon()->GetInfo()->GetSkill();
		std::vector<PokemonSkill*>& PoeSkill = PoeCurrentPokemon_->GetPokemon()->GetInfo()->GetSkill();
		for (auto& Skill : PlayerSkill)
		{
			if (Skill->GetNameConstRef() == _PlayerSkill)
			{
				Bool = true;
				break;
			}
		}

		for (auto& Skill : PoeSkill)
		{
			if (Skill->GetNameConstRef() == _PoeSkill)
			{
				Bool = true;
				break;
			}
		}

		if (Bool == false)
		{
			MsgBoxAssert("�ش� ���ϸ��� ��ų�� ������ ���� �ʽ��ϴ�");
		}
	}

	//BattleManager_ = new BattleManager(_PlayerSkill, _PoeSkill, BattleData_);
	BState_ = BattleState::BattlePage;
}

void BattleLevel::EndBattlePage()
{
	if (BattleManager_ != nullptr)
	{
		delete BattleManager_;
		BattleManager_ = nullptr;
	}
	
}

void BattleLevel::LevelChangeStart(GameEngineLevel * _PrevLevel)
{

	if (PlayerRed_ == nullptr)
	{
		PlayerRed_  = PlayerRed::MainRed_;
	}

	//BState_ = BattleState::Openning
	{
		BState_ = BattleState::SelecetPage;
		OpenningEnd_ = false;
		EnddingEnd_ = false;
		ShowOpenning();
	}



	// ������ : ��Ʋ �����
	{
		Opponent_ = CreateActor<BattleNPCInterface>(0, "Debug");
		Opponent_->PushPokemon(PokemonInfoManager::GetInst().CreatePokemon("Charmander"));
		PlayerRed_->GetPokemonList().push_back(PokemonInfoManager::GetInst().CreatePokemon("Squirtle"));

		BattleData_ = new BattleData(PlayerRed_, Opponent_, this);
		RefreshPokemon();
	}
}

void BattleLevel::ShowOpenning()
{

}


void BattleLevel::LevelChangeEnd(GameEngineLevel* _NextLevel)
{
	OpenningEnd_ = false;
	EnddingEnd_ = false;

	// ������ : Debug
	{
		Opponent_->Death();
		Opponent_->GetPokemonList()[0]->Death();
		PlayerRed_->GetPokemonList().front()->Death();
		delete BattleData_;
	}
}

void BattleLevel::ShowEndding()
{
}

void BattleLevel::RefreshPokemon()
{
	PlayerCurrentPokemon_ = BattleData_->GetCurrentPlayerPokemon();
	PoeCurrentPokemon_ = BattleData_->GetCurrentPoePokemon();
}


BattleData::BattleData(PlayerRed* _Player, BattleNPCInterface* _Poe, BattleLevel* _Level)
	: PlayerCurrentPokemonInBattle_(nullptr)
	, PoeCurrentPokemonInBattle_(nullptr)
	, PoeNPC_(_Poe)
	, Player_(_Player)
	, PlayerPokemonList_(_Player->GetPokemonList())
	, PoePokemonList_(_Poe->GetPokemonList())
	, WildBattle_(false)
{
	{
		// Player
		size_t PokemonInt = PlayerPokemonList_.size();
		for (size_t i = 0; i < PokemonInt; i++)
		{
			PlayerPokemonsInBattle_.push_back(CreatePokemonState(PlayerPokemonList_[i]));
		}
	}
	{
		// Poe
		size_t PokemonInt = PoePokemonList_.size();
		for (size_t i = 0; i < PokemonInt; i++)
		{
			PeoPokemonsInBattle_.push_back(CreatePokemonState(PoePokemonList_[i]));
		}
	}

	PlayerCurrentPokemonInBattle_ = PlayerPokemonsInBattle_.front();
	PoeCurrentPokemonInBattle_ = PeoPokemonsInBattle_.front();
}

BattleData::BattleData(PlayerRed* _Player, Pokemon* _WildPokemon, BattleLevel* _Level)
	: PlayerCurrentPokemonInBattle_(nullptr)
	, PoeCurrentPokemonInBattle_(nullptr)
	, PoeNPC_(nullptr)
	, Player_(_Player)
	, PlayerPokemonList_(_Player->GetPokemonList())
	, PoePokemonList_(_Level->CreateActor<WildPokemonNPC>(0, "WildPokemon")->GetPokemonList())
	, WildBattle_(true)
{
	// PlayerCurrentPokemonInBattle_ = _Player->GetPlayerPokemon
	PoeNPC_ = dynamic_cast<BattleNPCInterface*>(_Level->FindActor("WildPokemon"));
	
	{
		// Player
		size_t PokemonInt = PlayerPokemonList_.size();
		for (size_t i = 0; i < PokemonInt; i++)
		{
			PlayerPokemonsInBattle_.push_back(CreatePokemonState(PlayerPokemonList_[i]));
		}
	}

	{
		// Poe
		size_t PokemonInt = PoePokemonList_.size();
		for (size_t i = 0; i < PokemonInt; i++)
		{
			PeoPokemonsInBattle_.push_back(CreatePokemonState(PoePokemonList_[i]));
		}
	}

	PlayerCurrentPokemonInBattle_ = PlayerPokemonsInBattle_.front();
	PoeCurrentPokemonInBattle_ = PeoPokemonsInBattle_.front();
}

BattleData::~BattleData()
{
	if (WildBattle_ == true)
	{
		PoeNPC_->Death();
	}

	for (auto* State : AllPokemonInBattle_)
	{
		if (State != nullptr)
		{
			delete State;
			State = nullptr;
		}
	}
}

PokemonBattleState* BattleData::CreatePokemonState(Pokemon* _Pokemon)
{
	PokemonBattleState* PokemonState = new PokemonBattleState(_Pokemon);
	AllPokemonInBattle_.push_back(PokemonState);
	return PokemonState;
}

PokemonBattleState::PokemonBattleState(Pokemon* _Pokemon)
	: Pokemon_(_Pokemon)
	, CanAction_(true)
{
	{
		CurrentRank_[PokemonAbility::Att] = 0;
		CurrentRank_[PokemonAbility::Def] = 0;
		CurrentRank_[PokemonAbility::SpAtt] = 0;
		CurrentRank_[PokemonAbility::SpDef] = 0;
		CurrentRank_[PokemonAbility::Speed] = 0;
		CurrentRank_[PokemonAbility::Accuracy] = 0;
		CurrentRank_[PokemonAbility::Evasion] = 0;
	}
}

PokemonBattleState::~PokemonBattleState()
{
	for (auto* ApplySkill_ : AllCurrentApplySkill_)
	{
		if (ApplySkill_ != nullptr)
		{
			delete ApplySkill_;
		}
	}
}

bool PokemonBattleState::SetSkill(PokemonBattleState* _AlppyPokemon, PokemonSkill* _Skill)
{
	// �鿪�Ͻ� return false
	ApplySkill* MakeApplySkill = new ApplySkill(_AlppyPokemon, _Skill);
	AllCurrentApplySkill_.push_back(MakeApplySkill);
	return true;
}

float PokemonBattleState::GetRank(const PokemonAbility& _State)
{
	switch (_State)
	{
	case PokemonAbility::Att:
	case PokemonAbility::Def:
	case PokemonAbility::SpAtt:
	case PokemonAbility::SpDef:
	case PokemonAbility::Speed:
		switch (CurrentRank_[_State])
		{
		case -6:
			return 0.25f;
			break;
		case -5:
			return 0.29f;
			break;
		case -4:
			return 0.33f;
			break;
		case -3:
			return 0.40f;
			break;
		case -2:
			return 0.50f;
			break;
		case -1:
			return 0.66f;
			break;
		case 0:
			return 1;
			break;
		case 1:
			return 1.5f;
			break;
		case 2:
			return 2.0f;
			break;
		case 3:
			return 2.5f;
			break;
		case 4:
			return 3.0f;
			break;
		case 5:
			return 3.5f;
			break;
		case 6:
			return 4.0f;
			break;
		default:
			MsgBoxAssert("��ũ ��ġ�� �߸� �Ǿ����ϴ�")
				break;
		}
		break;
	case PokemonAbility::Accuracy:
	case PokemonAbility::Evasion:
		switch (CurrentRank_[_State])
		{
		case -6:
			return 0.33f;
			break;
		case -5:
			return 0.38f;
			break;
		case -4:
			return 0.43f;
			break;
		case -3:
			return 0.50f;
			break;
		case -2:
			return 0.60f;
			break;
		case -1:
			return 0.75f;
			break;
		case 0:
			return 1;
			break;
		case 1:
			return 1.33f;
			break;
		case 2:
			return 1.66f;
			break;
		case 3:
			return 2.0f;
			break;
		case 4:
			return 2.33f;
			break;
		case 5:
			return 2.66f;
			break;
		case 6:
			return 3.0f;
			break;
		default:
			MsgBoxAssert("��ũ ��ġ�� �߸� �Ǿ����ϴ�")
				break;
		}
		break;
	default:
		break;

	}
	return 100.0f;
}

void PokemonBattleState::Update()
{
	for (auto* ApplySkill_ : AllCurrentApplySkill_)
	{
		int LeftTurn = (ApplySkill_)->GetLeftTurn();
		if (LeftTurn == 0)
		{
			// Skill.second.second->End()
		}
		else if (LeftTurn <= -1)
		{
			// ���� ����
		}
		else
		{
			//(ApplySkill_)->Update();
			//(ApplySkill_)->TurnPass();
		}
	}
}

BattleManager::BattleManager(const std::string& _PlayerSkill, const std::string& _PoeSkill, BattleLevel* _Level)
	: PlayerSkill_(PokemonInfoManager::GetInst().FindSkill("_PlayerSkill"))
	, PoeSkill_(PokemonInfoManager::GetInst().FindSkill("_PoeSkill"))
	, PlayCurrentPokemon_(_Level->BattleData_->GetCurrentPlayerPokemon())
	, PoeCurrentPokemon_(_Level->BattleData_->GetCurrentPoePokemon())
	, Select_(BattleOrderMenu::Fight)
	, CurrentBattlePage_(BattlePage::FirstBattle)
	, PlayerFirst_(false)
	, Critical_(false)
	, Interface_(_Level->Interface_)
	, CurrentFont_(Battlefont::None)
{
	if (PlayerSkill_ == nullptr || PoeSkill_ == nullptr)
	{
		MsgBoxAssert("��ų���� ��ġ���� �ʽ��ϴ�")
	}

	switch (Select_)
	{
	case BattleOrderMenu::Run:
	case BattleOrderMenu::Fight:
		PlayerFirst_ = BattleEngine::ComareSpeed(PlayCurrentPokemon_, PoeCurrentPokemon_);
		break;
	case BattleOrderMenu::Item:
		break;
	case BattleOrderMenu::Pokemon:
		break;

	default:
		break;
	}
	

}

bool BattleManager::Update()
{
	PokemonBattleState* CurrentTurn = nullptr;
	PokemonSkill* PokemonSkill = nullptr;
	if (PlayerFirst_ == true)
	{
		CurrentTurn = PlayCurrentPokemon_;
		PokemonSkill = PlayerSkill_;
	}
	else
	{
		CurrentTurn = PoeCurrentPokemon_;
		PokemonSkill = PoeSkill_;
	}
	switch (CurrentBattlePage_)
	{
	case BattlePage::FirstBattle:
		switch (Select_)
		{
		case BattleOrderMenu::Fight:
			switch (CurrentFont_)
			{
			case Battlefont::None:
				Interface_->ShowUsedSkillString(CurrentTurn->GetPokemon()->GetInfo()->GetNameConstRef(), PokemonSkill->GetNameConstRef());
				break;
			case Battlefont::Att:
				break;
			case Battlefont::Wait:
				break;
			case Battlefont::Effect:
				break;
			default:
				break;
			}
			break;
		case BattleOrderMenu::Item:
			break;
		case BattleOrderMenu::Pokemon:
			break;
		case BattleOrderMenu::Run:
			break;
		default:
			break;
		}
		
		break;
	case BattlePage::SecondBattle:
		break;
	case BattlePage::End:
		break;
	}
	return false;
}
