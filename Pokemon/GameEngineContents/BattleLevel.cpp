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
#include <GameEngine/GameEngine.h>
#include "Green.h"
#include "NPCBrock.h"



BattleLevel::BattleLevel()
	: Interface_(nullptr)
	, BState_(BattleState::Openning)
	, OpenningEnd_(false)
	, EnddingEnd_(false)
	// 디버깅
	, PlayerCurrentPokemon_(nullptr)
	, PoeCurrentPokemon_(nullptr)
	, PlayerRed_(nullptr)
	, Opponent_(nullptr)
	, UnitRenderer(nullptr)
	, OneTalk(false)
	, Fonts(nullptr)
	, BattleData_(nullptr)
	, BattleManager_(nullptr)
	, CurrentSelect_(BattleOrder::None)
	, EndFont_(false)
	, EndAction_(BattlePageEnd::None)
	, DebugMode_(false)
	, WildBattle_(false)
	, DoingSkillAnimation_(false)
{

}

BattleLevel::~BattleLevel()
{
	if (BattleData_ != nullptr)
	{
		delete BattleData_;
		BattleData_ = nullptr;
	}
	if (Opponent_ != nullptr)
	{
		//
		////		delete PlayerRed_->GetPokemonList().front();
		//PlayerRed_ = nullptr;
		//Opponent_ = nullptr;
	}

	if (BattleManager_ != nullptr)
	{
		delete BattleManager_;
		BattleManager_ = nullptr;
	}
}

void BattleLevel::Loading()
{

	/// 장중혁 : BattleTestNPC1 DebugTest
	//Pokemon* Debug = CreateActor<Pokemon>();
	//Debug->SetInfo("Squirtle");
	//Opponent_ = CreateActor<BattleTestNPC1>();
	//Opponent_->PushPokemon(Debug);
	//PlayerCurrentPokemon_ = CreateActor<Pokemon>();
	//PlayerCurrentPokemon_->SetInfo("Charmander");
	//PoeCurrentPokemon_ = Opponent_->GetPokemonList();
	// Debug



	CreateActor<BattleBackground>();
	UnitRenderer = CreateActor<BattleUnitRenderer>(3);
	RegistActor("BattleUnitRenderer", UnitRenderer);
	Interface_ = CreateActor<BattleInterface>(3);
	Interface_->SetPosition({ 720.0f, 548.0f });
	DebugMode_ = true;

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
		}
		break;
	case BattleState::BattlePage:
		// Update();
		if (Interface_->BattleKey() && DoingSkillAnimation_ == false /*EndFont_ == true*/)
		{
			switch (EndAction_)
			{
			case BattlePageEnd::None:
				break;
			case BattlePageEnd::GoEnd:
				BState_ = BattleState::Endding;
				return;
				break;
			case BattlePageEnd::GoSelecet:
				BState_ = BattleState::SelecetPage;
				UnitRenderer->SetFighting(false);
				Interface_->GetSelect()->SetPivot({-190.0f, -25.0f});
				EndAction_ = BattlePageEnd::None;
				return;
				break;
			case BattlePageEnd::ChangePokemon:
				Interface_->BattleStatus_->Off();
				for (auto Iter : Interface_->AllLevelUpFont_)
				{
					Iter->EndFont();
				}
				Interface_->ShowChangePokemon(BattleData_->GetNameString(), BattleData_->GetCurrentPoePokemon()->GetPokemon()->GetInfo()->GetNameConstRef());
				BattleData_->GetCurrentPlayerPokemon()->ResetRank();
				BattleData_->GetCurrentPoePokemon()->ResetRank();
				UnitRenderer->SetPoeRenderer();
				if (BattleManager_ != nullptr)
				{
					delete BattleManager_;
					BattleManager_ = nullptr;
				}
				EndAction_ = BattlePageEnd::GoSelecet;
				return;
				break;
			case BattlePageEnd::SetPokemon:
				break;
			case BattlePageEnd::CatchPokeBall:
				Interface_->ShowGotPokemonByBall(BattleData_->GetCurrentPoePokemon()->GetPokemon()->GetInfo()->GetNameConstRef());
				PlayerRed_->AddPokemon(PoeCurrentPokemon_->GetPokemon());
				EndAction_ = BattlePageEnd::GoEnd;
				return;
				break;
			case BattlePageEnd::LevelUp:
			{
				//
				LevelUp(BattleData_->GetCurrentPlayerPokemon());
				Interface_->LevelStatu();
				EndAction_ = BattlePageEnd::LevelUpState;
				// 레벨업 어쩌구 삽입
			}
				return;
				break;
			case BattlePageEnd::LevelUpState:
			{
				Interface_->ShowLevelUp(BattleData_->GetCurrentPlayerPokemon()->GetPokemon()->GetInfo()->GetNameConstRef(), BattleData_->GetCurrentPlayerPokemon()->GetPokemon()->GetInfo()->GetMyLevel(), true);
				if (BattleData_->IsWild())
				{
					BState_ = BattleState::Endding;
					return;
					break;
				}
				else
				{
					const std::vector<PokemonBattleState*>& PoePokemonList = BattleData_->GetCurrentPoePokemonList();
					for (auto Pokemon : PoePokemonList)
					{
						if (!Pokemon->GetPokemon()->GetInfo()->GetFaint())
						{
							BattleData_->SetCurrentPoePokemon(Pokemon);
							EndAction_ = BattlePageEnd::ChangePokemon;
							return;
							break;
						}
					}
					BState_ = BattleState::Endding;
				}
			}
			break;
			default:
				break;
			}
			switch (BattleManager_->Update())
			{
			case InBattle::Wait:
				break;
			case InBattle::BattleEnd:
				EndBattlePage();
				break;
			case InBattle::BattleEndByPlayerDeath:
				break;
			case InBattle::Pokeball:
				EndAction_ = BattlePageEnd::CatchPokeBall;
				break;
			case InBattle::BattleEndByPoeDeath:
				if (BattleData_->GetCurrentPlayerPokemon()->GetPokemon()->GetInfo()->GetMaxExp() <= BattleData_->GetCurrentPlayerPokemon()->GetPokemon()->GetInfo()->GetExp())
				{
					EndAction_ = BattlePageEnd::LevelUp;
					return;
					break;
				}
				if (BattleData_->IsWild())
				{
					// 경험치 얻고 끝
					BState_ = BattleState::Endding;
					return;
					break;
					//EndBattle
				}
				else
				{
					const std::vector<PokemonBattleState*>& PoePokemonList = BattleData_->GetCurrentPoePokemonList();
					for (auto Pokemon : PoePokemonList)
					{
						if (!Pokemon->GetPokemon()->GetInfo()->GetFaint())
						{
							BattleData_->SetCurrentPoePokemon(Pokemon);
							EndAction_ = BattlePageEnd::ChangePokemon;
							return;
							break;
						}
					}
					BState_ = BattleState::Endding;
					return;
				}
				break;
			default:
				break;
			}
		}
		return;
		break;
	case BattleState::Endding:
		//if (EnddingEnd_ == true)
		//{
		if (Interface_->BattleKey() || EndFont_ == true)
		{
			GameEngine::GetInst().ChangeLevel("WorldMap");
			return;
		}
		//}
		break;
	}
}

void BattleLevel::LevelUp(PokemonBattleState* _PlayerPokemon)
{
	int CurrentEXP = _PlayerPokemon->GetPokemon()->GetInfo()->GetExp();
	int MaxEXP = _PlayerPokemon->GetPokemon()->GetInfo()->GetMaxExp();
	CurrentEXP = CurrentEXP - MaxEXP;
	_PlayerPokemon->GetPokemon()->GetInfo()->SetExp(CurrentEXP);
	int CurrntLevel = _PlayerPokemon->GetPokemon()->GetInfo()->GetMyLevel();
	++CurrntLevel;
	_PlayerPokemon->GetPokemon()->GetInfo()->SetMyLevel(CurrntLevel);

	Interface_->ShowLevelUp(_PlayerPokemon->GetPokemon()->GetInfo()->GetNameConstRef(), CurrntLevel);

}

void BattleLevel::StartBattlePage(PokemonSkillInfo* _PlayerSkill, PokemonSkillInfo* _PoeSkill)
{
	RefreshPokemon();


	BattleManager_ = new BattleManager(_PlayerSkill, _PoeSkill, this);
	BState_ = BattleState::BattlePage;
	BattleManager_->Update();
}

void BattleLevel::StartBattlePage()
{
	RefreshPokemon();

	BattleManager_ = new BattleManager("PokeBall", this);
	BState_ = BattleState::BattlePage;
	//BattleManager_->Update();
}

void BattleLevel::EndBattlePage()
{
	if (BattleManager_ != nullptr)
	{
		delete BattleManager_;
		BattleManager_ = nullptr;
	}
	Interface_->BattleUnit->SetFighting(false);
	Interface_->GetSelect()->SetPivot({-190.0f, -25.0f});

	BState_ = BattleState::SelecetPage;
	GameEngineInput::GetInst()->Reset();
	EndAction_ = BattlePageEnd::None;
}

void BattleLevel::LevelChangeStart(GameEngineLevel* _PrevLevel)
{
	//김예나 : 이 브금은 실제론 월드맵에서 전투 걸렸을때부터 시작되야 합니다.
	BState_ = BattleState::Openning;
	//BgmPlayer_ = GameEngineSound::SoundPlayControl("Wild_Battle.mp3");
	//BgmPlayer_.SetVolume(0.1f);

	if (PlayerRed_ == nullptr)
	{
		PlayerRed_ = PlayerRed::MainRed_;
	}

	//BState_ = BattleState::Openning
	{
		//BState_ = BattleState::SelecetPage;
		OpenningEnd_ = false;
		EnddingEnd_ = false;
		ShowOpenning();
	}

	if (DebugMode_)
	{
		LevelStartDebug();
	}
	RefreshPokemon();
	EndFont_ = false;

}

void BattleLevel::ShowOpenning()
{
	//bool 값을 넣어서 한번만 실행되도록..
}


void BattleLevel::LevelChangeEnd(GameEngineLevel* _NextLevel)
{
	BgmPlayer_.StopWithNullCheck();

	OpenningEnd_ = false;
	EnddingEnd_ = false;

	// 장중혁 : Debug
	{

		if (DebugMode_)
		{
			LevelEndDebug();
		}
		delete BattleData_;
		BattleData_ = nullptr;
	}

	if (BattleManager_ != nullptr)
	{
		delete BattleManager_;
		BattleManager_ = nullptr;
	}

	WildBattle_ = false;
	DoingSkillAnimation_ = false;
	EndAction_ = BattlePageEnd::None;
	DebugMode_ = true;
}

void BattleLevel::LevelStartDebug()
{
	//Opponent_ = CreateActor<BattleNPCInterface>(0, "Debug");
	//Opponent_->PushPokemon(PokemonInfoManager::GetInst().CreatePokemon("Charmander"));
	Opponent_ = nullptr;
	BattleData_ = new BattleData(PlayerRed_ ,PokemonInfoManager::GetInst().CreatePokemon("Charmander"), this);
}
void BattleLevel::LevelEndDebug()
{
	if (Opponent_ != nullptr)
	{
		for (auto& Iter : Opponent_->GetPokemonList())
		{
			PokemonInfoManager::GetInst().DestroyPokemon(Iter->GetInfo()->GetMyId());
		}
		Opponent_->GetPokemonList().clear();
		Opponent_->GetActor()->Death();
	}
}

void BattleLevel::StartBattleLevelByWild()
{

	if (PlayerRed_ == nullptr)
	{
		PlayerRed_ = PlayerRed::MainRed_;
	}

	Pokemon* WildPokemon = nullptr;
	WildPokemon = GameEngineRandom::GetInst_->RandomInt(0, 1) == 0 ? PokemonInfoManager::GetInst().CreatePokemon("Rattata") : PokemonInfoManager::GetInst().CreatePokemon("Pidgey");
	BattleData_ = new BattleData(PlayerRed_, WildPokemon, this);
	DebugMode_ = false;
	WildBattle_ = true;
	GameEngine::GetInst().ChangeLevel("Battle"); 
}

void BattleLevel::StartBattleLevelByNPC(BattleNPCInterface* _Opponent)
{
	if (!_Opponent->IsBattleNPC() || _Opponent->GetPokemonList().empty())
	{
		MsgBoxAssert("배틀 NPC가 아니거나 포켓몬을 가지고 있지 않습니다")
	}
	if (PlayerRed_ == nullptr)
	{
		PlayerRed_ = PlayerRed::MainRed_;
	}
	Opponent_ = _Opponent;
	DebugMode_ = false;
	WildBattle_ = false;
	BattleData_ = new BattleData(PlayerRed_, Opponent_, this);
	GameEngine::GetInst().ChangeLevel("Battle");
}

void BattleLevel::ShowEndding()
{
}

void BattleLevel::RefreshPokemon()
{
	if (BattleData_ != nullptr)
	{
		PlayerCurrentPokemon_ = BattleData_->GetCurrentPlayerPokemon();
		PoeCurrentPokemon_ = BattleData_->GetCurrentPoePokemon();
	}
}


BattleData::BattleData(PlayerRed* _Player, BattleNPCInterface* _Poe, BattleLevel* _Level)
	: PlayerCurrentPokemonInBattle_(nullptr)
	, PoeCurrentPokemonInBattle_(nullptr)
	, PoeNPC_(_Poe)
	, Player_(_Player)
	, PlayerPokemonList_(_Player->GetPokemonList())
	, PoePokemonList_(_Poe->GetPokemonList())
	, WildBattle_(false)
	, PoeNPCName_("")
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

	if (dynamic_cast<Green*>(PoeNPC_) != nullptr)
	{
		PoeNPCName_ = "Green";
	}
	else if (dynamic_cast<NPCBrock*>(PoeNPC_) != nullptr)
	{
		PoeNPCName_ = "Brock";
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
		PoePokemonList_.push_back(_WildPokemon);
		PeoPokemonsInBattle_.push_back(CreatePokemonState(PoePokemonList_[0]));

	}

	PlayerCurrentPokemonInBattle_ = PlayerPokemonsInBattle_.front();
	PoeCurrentPokemonInBattle_ = PeoPokemonsInBattle_.front();
}

BattleData::~BattleData()
{
	if (PoeNPC_ != nullptr && WildBattle_ == true)
	{
		PoeNPC_->GetActor()->Death();
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
	ResetRank();
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

bool PokemonBattleState::SetSkill(PokemonBattleState* _AlppyPokemon, PokemonSkillInfo* _Skill)
{
	// 면역일시 return false
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
			MsgBoxAssert("랭크 수치가 잘못 되었습니다")
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
			MsgBoxAssert("랭크 수치가 잘못 되었습니다")
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
			// 무한 지속
		}
		else
		{
			//(ApplySkill_)->Update();
			//(ApplySkill_)->TurnPass();
		}
	}
}

BattleManager::BattleManager(PokemonSkillInfo* _PlayerSkill, PokemonSkillInfo* _PoeSkill, BattleLevel* _Level)
	: PlayerSkill_(_PlayerSkill)
	, PoeSkill_(_PoeSkill)
	, Level_(_Level)
	, PlayCurrentPokemon_(_Level->BattleData_->GetCurrentPlayerPokemon())
	, PoeCurrentPokemon_(_Level->BattleData_->GetCurrentPoePokemon())
	, Select_(BattleOrder::Fight)
	, CurrentBattlePage_(BattlePage::FirstBattle)
	, PlayerFirst_(false)
	, Interface_(_Level->Interface_)
	, CurrentFont_(Battlefont::None)
	, FristTurn_(nullptr)
	, SecondTurn_(nullptr)
	, DeadSwitch_(false)
	, UsePokemonBall_(false)
{

	switch (Select_)
	{
	case BattleOrder::Run:
	case BattleOrder::Fight:
		PlayerFirst_ = BattleEngine::ComareSpeed(PlayCurrentPokemon_, PoeCurrentPokemon_);
		break;
	case BattleOrder::Bag:
		break;
	case BattleOrder::Pokemon:
		break;

	default:
		break;
	}


}

BattleManager::BattleManager(const std::string& _PlayerItem, BattleLevel* _Level)
	: PlayerSkill_(nullptr)
	, PoeSkill_(nullptr)
	, Level_(_Level)
	, PlayCurrentPokemon_(_Level->BattleData_->GetCurrentPlayerPokemon())
	, PoeCurrentPokemon_(_Level->BattleData_->GetCurrentPoePokemon())
	, Select_(BattleOrder::Bag)
	, CurrentBattlePage_(BattlePage::FirstBattle)
	, PlayerFirst_(true)
	, Interface_(_Level->Interface_)
	, CurrentFont_(Battlefont::None)
	, FristTurn_(nullptr)
	, SecondTurn_(nullptr)
	, DeadSwitch_(false)
	, UsePokemonBall_(true)
	, GetExp_(false)
{

}


void BattleManager::PlayerPokemonDead()
{
	Interface_->ShowPlayerFaintString(PlayCurrentPokemon_->GetPokemon()->GetInfo()->GetNameConstRef());
	DeadSwitch_ = true;
}

void BattleManager::PoePokemonDead()
{
	Interface_->ShowPoeFaintString(PoeCurrentPokemon_->GetPokemon()->GetInfo()->GetNameConstRef());
	PoeCurrentPokemon_->GetPokemon()->GetInfo()->SetFaint(true);
	DeadSwitch_ = true;
}


InBattle BattleManager::Update()
{
	if (UsePokemonBall_ == true)
	{
		if (Level_->DoingSkillAnimation_ == true || Level_->EndAction_ == BattlePageEnd::CatchPokeBall)
		{
			return InBattle::Wait;
		}
		Interface_->ShowUsePokeball(); 
		Level_->UnitRenderer->SkillName_ = SkillName::Catch;
		Level_->UnitRenderer->MyCatchEnd = false;
		Level_->DoingSkillAnimation_ = true;
		return InBattle::Pokeball;
	}
	PokemonBattleState* CurrentTurn = PlayerFirst_ == true ? PlayCurrentPokemon_ : PoeCurrentPokemon_;
	PokemonSkillInfo* CurrentPokemonSkill = PlayerFirst_ == true ? PlayerSkill_ : PoeSkill_;

	PokemonBattleState* AfterTrun = PlayerFirst_ == false ? PlayCurrentPokemon_ : PoeCurrentPokemon_;
	PokemonSkillInfo* AfterPokemonSkill = PlayerFirst_ == false ? PlayerSkill_ : PoeSkill_;

	if (PlayCurrentPokemon_->GetPokemon()->GetInfo()->GetHp() <= 0)
	{
		if (DeadSwitch_ == true)
		{
			return InBattle::BattleEndByPlayerDeath;
		}
		else
		{
			PlayerPokemonDead();
			return InBattle::Wait;
		}
	
	}
	else if (PoeCurrentPokemon_->GetPokemon()->GetInfo()->GetHp() <= 0)
	{
		if (DeadSwitch_ == true && GetExp_ == false)
		{
			int EXP = GameEngineRandom::GetInst_->RandomInt(80, 100);
			Interface_->ShowGetEXP(PlayCurrentPokemon_->GetPokemon()->GetInfo()->GetNameConstRef(), EXP);
			PlayCurrentPokemon_->GetPokemon()->GetInfo()->PlusExp(EXP);
			GetExp_ = true;
			return InBattle::BattleEndByPoeDeath;
		}
		else if (DeadSwitch_ == false)
		{
			PoePokemonDead();
			return InBattle::Wait;
		}
		return InBattle::BattleEndByPoeDeath;
	}

	switch (CurrentBattlePage_)
	{
	case BattlePage::FirstBattle:
		switch (Select_)
		{
		case BattleOrder::Fight:
			if (FristTurn_ == nullptr)
			{
				FristTurn_ = new BattleTurn(CurrentTurn, AfterTrun, CurrentPokemonSkill);
			}
			if (CheckBattle(CurrentTurn, AfterTrun, CurrentPokemonSkill, FristTurn_))
			{
				delete FristTurn_;
				FristTurn_ = nullptr;
				CurrentBattlePage_ = BattlePage::SecondBattle;
			}
			break;
		case BattleOrder::Bag:
			break;
		case BattleOrder::Pokemon:
			break;
		case BattleOrder::Run:
			break;
		default:
			break;
		}
		break;
	case BattlePage::SecondBattle:
		switch (Select_)
		{
		case BattleOrder::Fight:
			if (SecondTurn_ == nullptr)
			{
				SecondTurn_ = new BattleTurn(CurrentTurn, AfterTrun, CurrentPokemonSkill);
			}
			if (CheckBattle(CurrentTurn, AfterTrun, CurrentPokemonSkill, SecondTurn_))
			{
				delete SecondTurn_;
				SecondTurn_ = nullptr;
				CurrentBattlePage_ = BattlePage::End;
			}
			break;
		case BattleOrder::Bag:
			break;
		case BattleOrder::Pokemon:
			break;
		case BattleOrder::Run:
			break;
		default:
			break;
		}
		break;
	case BattlePage::End:
		return InBattle::BattleEnd;
		break;
	}
	return InBattle::Wait;
}

bool BattleManager::CheckBattle(PokemonBattleState* _Att, PokemonBattleState* _Def, PokemonSkillInfo* _Skill, BattleTurn* _Turn)
{
	SkillType AttSkillType = _Turn->SkillType_;


	switch (CurrentFont_)
	{
	case Battlefont::None:
		Interface_->ShowUsedSkillString(_Att->GetPokemon()->GetInfo()->GetNameConstRef(), _Skill->GetNameConstRef());
		CurrentFont_ = Battlefont::Att;
		break;

	case Battlefont::Miss:
		break;

	case Battlefont::Att:
	{
		const std::string& SkillName = _Skill->GetNameConstRef();
		switch (AttSkillType)
		{
		case SkillType::Physical:
		case SkillType::Special:
		{
			//이펙트
			//포켓몬 흔들고 체력 다는 효과 추가
			if (PlayerFirst_ ==  true)
			{
				if (SkillName == "TACKLE")
				{
					GameEngineSound::SoundPlayOneShotWithVolume("Tackle.mp3", 0.5f);
					Level_->UnitRenderer->SkillName_ = SkillName::Tackle;
					Level_->UnitRenderer->MyTurnEnd = false;
					Level_->DoingSkillAnimation_ = true;
				}
				else if (SkillName == "WATERGUN")
				{
					GameEngineSound::SoundPlayOneShotWithVolume("Water_Gun.mp3", 0.5f);
					Level_->UnitRenderer->SkillName_ = SkillName::WaterGun;
					Level_->UnitRenderer->MyTurnEnd = false;
					Level_->DoingSkillAnimation_ = true;
				}

			}
			else if (PlayerFirst_ == false)
			{
				if (SkillName == "TACKLE")
				{
					GameEngineSound::SoundPlayOneShotWithVolume("Tackle.mp3", 0.5f);
					Level_->UnitRenderer->SkillName_ = SkillName::EnemyTackle;
					Level_->UnitRenderer->EnemyTurnEnd = false;
					Level_->DoingSkillAnimation_ = true;
				}
				else if (SkillName == "ROCKTOMB")	
				{
					GameEngineSound::SoundPlayOneShotWithVolume("Rock_Tomb.mp3", 0.5f);
					Level_->UnitRenderer->SkillName_ = SkillName::EnemyRock;
					Level_->UnitRenderer->EnemyTurnEnd = false;
					Level_->DoingSkillAnimation_ = true;
				}
				else if (SkillName == "SCRATCH")
				{
					GameEngineSound::SoundPlayOneShotWithVolume("Scratch.mp3", 0.5f);
					Level_->UnitRenderer->SkillName_ = SkillName::EnemyScratch;
					Level_->UnitRenderer->EnemyTurnEnd = false;
					Level_->DoingSkillAnimation_ = true;
				}
			}

			CurrentFont_ = Battlefont::Wait;
		}

		break;
		case SkillType::Status:
		{
			if (PlayerFirst_ == true)
			{
				if (_Skill->GetNameConstRef() == "TAILWHIP")
				{
					GameEngineSound::SoundPlayOneShotWithVolume("Tail_Whip.mp3", 0.5f);
					Level_->UnitRenderer->SkillName_ = SkillName::TailWhipMove;
					Level_->UnitRenderer->MyTurnEnd = false;
					Level_->DoingSkillAnimation_ = true;
					//TailWhipMove();
				}
				else if (_Skill->GetNameConstRef() == "GROWL")
				{
					GameEngineSound::SoundPlayOneShotWithVolume("Growl.mp3", 0.5f);
					Level_->UnitRenderer->SkillName_ = SkillName::MyGrowl;
					Level_->UnitRenderer->MyTurnEnd = false;
					Level_->DoingSkillAnimation_ = true;
				}
				else if (_Skill->GetNameConstRef() == "WITHDRAW")
				{
					GameEngineSound::SoundPlayOneShotWithVolume("Withdraw.mp3", 0.5f);
					Level_->UnitRenderer->SkillName_ = SkillName::ShellHide;
					Level_->UnitRenderer->MyTurnEnd = false;
					Level_->DoingSkillAnimation_ = true;
					
				}
			}
			else
			{
				if (_Skill->GetNameConstRef() == "GROWL")
				{
					GameEngineSound::SoundPlayOneShotWithVolume("Growl.mp3", 0.5f);
				Level_->UnitRenderer->SkillName_ = SkillName::EnemyGrowl;
				Level_->UnitRenderer->EnemyTurnEnd = false;
				Level_->DoingSkillAnimation_ = true;
				}
			}
			CurrentFont_ = Battlefont::Wait;
		}

			// 꼬리흔들기 등 이펙트 구현, 포켓몬에게 랭크 적용
			break;
		default:
			break;
		}
	}

	// 어택 모션
	break;
	case Battlefont::Wait:
		if (_Turn->Critical_ == true)
		{
			Interface_->ShowCriticalHitString();
			FristTurn_->Critical_ = false;
		}
		else if (AttSkillType == SkillType::Status)
		{
			if (_Skill->GetNameConstRef() == "TAILWHIP")
			{
				Interface_->ShowRankUpAndDown(_Def->Pokemon_->GetInfo()->GetNameConstRef(), PokemonAbility::Def, -1);
				_Def->SetRank(PokemonAbility::Def, -1);
			}
			else if (_Skill->GetNameConstRef() == "GROWL")
			{
				Interface_->ShowRankUpAndDown(_Def->Pokemon_->GetInfo()->GetNameConstRef(), PokemonAbility::Att, -1);
				_Def->SetRank(PokemonAbility::Att, -1);
			}
			else if (_Skill->GetNameConstRef() == "WITHDRAW")
			{
				Interface_->ShowRankUpAndDown(_Def->Pokemon_->GetInfo()->GetNameConstRef(), PokemonAbility::Def, 1);
				_Att->SetRank(PokemonAbility::Def, 1);
			}
			PlayerFirst_ = !PlayerFirst_;
			CurrentFont_ = Battlefont::None;
			return true;
		}
		else
		{
			{
				switch (AttSkillType)
				{
				case SkillType::Physical:
				case SkillType::Special:
					_Turn->DamageType_ = BattleEngine::ComparePokemonType(_Skill, _Def);
					_Turn->FinalDamage_ = BattleEngine::AttackCalculation(_Att, _Def, _Skill, _Turn->DamageType_);
					// Interface_
					_Def->GetPokemon()->GetInfo()->GetHp() -= _Turn->FinalDamage_;
				{
					switch (_Turn->DamageType_)
					{
					case DamageType::Nomal:
						GameEngineSound::SoundPlayOneShotWithVolume("Hit_Normal_Damage.mp3", 0.5f);
						break;
					case DamageType::Great:
						GameEngineSound::SoundPlayOneShotWithVolume("Hit_Super_Effective.mp3", 0.5f);
						Interface_->ShowSupperEffectString();
						break;
					case DamageType::Bad:
						GameEngineSound::SoundPlayOneShotWithVolume("Hit_Weak_Not_Very_Effective.mp3", 0.5f);
						Interface_->ShowNotEffective();
						break;
					case DamageType::Nothing:
						Interface_->ShowFailed();
						break;
					default:
						break;
					}
				}
				break;
				case SkillType::Status:

					break;
				default:
					break;
				}
			}
			CurrentFont_ = Battlefont::None;
			PlayerFirst_ = !PlayerFirst_;
			return true;
		}
		//효과는 대단했다
		break;
	case Battlefont::Effect:
		break;
	default:
		break;
	}
	return false;
}

BattleManager::BattleTurn::BattleTurn(PokemonBattleState* const _Att, PokemonBattleState* const _Def, PokemonSkillInfo* _Skill)
	: AttPokemon_(_Att)
	, DefPokemon_(_Def)
	, FinalDamage_(0)
	, Critical_(false)
	, DamageType_(DamageType::Nomal)
	, SkillType_(_Skill->GetSkillType())
	, Skill_(_Skill)
{
}