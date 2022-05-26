#pragma once
#include <GameEngine/GameEngineLevel.h>
#include "BattleNPCInterface.h"
#include "PlayerRed.h"
#include "BattleInterface.h"
#include "BattleEnum.h"
#include "Pokemon.h"
#include <set>

//�������� ������ �͵鵵 ��������� ĥ ���̴�
//���� ������ �ʾƵ� �ڵ����� �����Ǿ� ������ �ִ°͵�

//���� : 

enum class PokemonAbility;
enum class DamageType;
enum class BattlePage;
enum class Battlefont;
class BattleEngine;
class BattleData;
class BattleManager;
class PokemonBattleState;
class BattleLevel : public GameEngineLevel
{
	friend class BattleInterface;
	friend BattleManager;
public:
	//����Ʈ ������
	BattleLevel();
	//����Ʈ �Ҹ���
	~BattleLevel();
	
	//======�Ʒ��͵��� ��������� �Ⱦ��ڽ��ϴ�(delete)======
	
	//����Ʈ ���� ������
	BattleLevel(const BattleLevel& _Other) = delete;
	//RValue Reference ������ (���߿� ����)
	BattleLevel(BattleLevel&& _Other) noexcept = delete;
	//operater= (�ڱ��ڽ��� �����ϴ�)
	BattleLevel& operator=(const BattleLevel& _Other) = delete;
	BattleLevel& operator=(BattleLevel&& _Other) noexcept = delete;

	inline BattleState GetBattleState() const
	{
		return BState_;
	}


public:
	inline void OpenningEnd()
	{
		OpenningEnd_ = true;
	}
	inline void EnddingEnd()
	{
		EnddingEnd_ = true;	
	}

protected:
	void Loading() override;
	void Update() override;

	void LevelChangeStart(GameEngineLevel* _PrevLevel) override;
	void LevelChangeEnd(GameEngineLevel* _NextLevel) override;

private:
	BattleInterface* Interface_;
	BattleState BState_;


	// ������ ����
	void ShowOpenning();
	bool OpenningEnd_;
	//
	
	// ���� ����
	void ShowEndding();
	bool EnddingEnd_;
	//
	



	//������ ��Ʈ ���
	class GameEngineContentFont* Fonts;
	class BattleUnitRenderer* PlayerStopCheck;

	bool OneTalk;



private:
	// �÷��̾� �� NPC
	PlayerRed* PlayerRed_;
	BattleNPCInterface* Opponent_;
	// Pokemon
	PokemonBattleState* PlayerCurrentPokemon_; // Debug
	PokemonBattleState* PoeCurrentPokemon_;// Debug
	//

	BattleData* BattleData_;
	BattleManager* BattleManager_;
	
	void RefreshPokemon();
	

	void StartBattlePage(const std::string& _PlayerSkill, const std::string& _PoeSkill);
	void EndBattlePage();
};

class BattleData
{
	friend BattleManager;
public:
	BattleData(PlayerRed* _Player, BattleNPCInterface* _Poe, BattleLevel* _Level);
	BattleData(PlayerRed* _Player, Pokemon* _WildPokemon, BattleLevel* _Level);
	~BattleData();

	inline PokemonBattleState* GetCurrentPlayerPokemon() const
	{
		return PlayerCurrentPokemonInBattle_;
	}

	inline PokemonBattleState* GetCurrentPoePokemon() const
	{
		return PoeCurrentPokemonInBattle_;
	}

private:

	BattleData(const BattleData& _Other) = delete;
	BattleData(BattleData&& _Other) noexcept = delete;
	BattleData& operator=(const BattleData& _Other) = delete;
	BattleData& operator=(BattleData&& _Other) noexcept = delete;

private:
	PlayerRed* Player_;
	BattleNPCInterface* PoeNPC_;

	// Data ���

	// �ָӴϼ� ������ �ִ� ���ϸ�
	std::vector<Pokemon*>& PlayerPokemonList_;
	std::vector<Pokemon*>& PoePokemonList_;


	// PokemonBatleState
	PokemonBattleState* PlayerCurrentPokemonInBattle_;
	PokemonBattleState* PoeCurrentPokemonInBattle_;


	std::vector<PokemonBattleState*> PlayerPokemonsInBattle_;
	std::vector<PokemonBattleState*> PeoPokemonsInBattle_;

	std::vector<PokemonBattleState*> AllPokemonInBattle_;

	bool WildBattle_;

	PokemonBattleState* CreatePokemonState(Pokemon* _Pokemon);
};

class PokemonBattleState
{
	friend BattleEngine;
	friend BattleManager;
	// ��ø Ŭ���� ���漱��
	class ApplySkill;
	// PokemonBattleState();
public:
	PokemonBattleState(Pokemon* _Pokemon);
	~PokemonBattleState();

	int SetRank(const PokemonAbility& _State, int _Value)
	{
		CurrentRank_[_State] += _Value;
		if (CurrentRank_[_State] > 6)
		{
			return CurrentRank_[_State] = 6;
		}
		else if (CurrentRank_[_State] < (-6))
		{
			return CurrentRank_[_State] = -6;
		}
		else
		{
			return CurrentRank_[_State];
		}
	}
	inline Pokemon* GetPokemon()
	{
		return Pokemon_;
	}
	float GetRank(const PokemonAbility& _State);
	// �� �� ���� �������� Ȯ���ϴ� �Լ� �ʿ�
	bool SetSkill(PokemonBattleState* _AlppyPokemon, PokemonSkillInfo* _Skill);
	void Update();

private:
	// ������ ���ϸ�
	Pokemon* const Pokemon_;

	// �ൿ ���ɿ���
	bool CanAction_;

	//���� ��ũ ����
	std::map<PokemonAbility, int> CurrentRank_;


	// PokemonSkill ��ӿ� virtual �Լ� Ing, End �ʿ�
	std::list<PokemonBattleState::ApplySkill*> AllCurrentApplySkill_;
	std::set<StatusEffect> ApplyStatus_;

	// ���� ����޴� ��ų��
	class ApplySkill
	{
	private:
		ApplySkill();
	public:
		ApplySkill(PokemonBattleState* _CastPokemon, PokemonSkillInfo* _Skill)
			: CastPokemon_(_CastPokemon)
			, Skill_(_Skill)
			, LeftTurn_(0)
		{
			// �ϼ� �ʿ�
			LeftTurn_ = 1; // ������ ��
		}
		~ApplySkill() {}

		inline void TurnPass()
		{
			LeftTurn_ -= 1;
		}

		inline int GetLeftTurn() const
		{
			return LeftTurn_;
		}

		inline PokemonSkillInfo* GetSkill() const
		{
			return Skill_;
		}

		inline PokemonBattleState* GetPokemon() const
		{
			return CastPokemon_;
		}

		void Update() const
		{
			//Skill_->;
		}
	private:
		PokemonBattleState* const CastPokemon_;
		PokemonSkillInfo* const Skill_;
		int LeftTurn_;
	};


};


class BattleManager
{
	class BattleTurn;
	friend BattleLevel;
private:
	BattleManager(const std::string& _PlayerSkill, const std::string& _PoeSkill, BattleLevel* _Level);
	//BattleManager(const std::string& _PlayerItem, const std::string& _PoeSkill);
	~BattleManager() {}

	BattleManager(const BattleManager& _Other) = delete;
	BattleManager(BattleManager&& _Other) noexcept = delete;
	BattleManager& operator=(const BattleManager& _Other) = delete;
	BattleManager& operator=(BattleManager&& _Other) noexcept = delete;

private:
	BattleInterface* const Interface_;

	PokemonSkillInfo* const PlayerSkill_;
	PokemonSkillInfo* const PoeSkill_;
	
	PokemonBattleState* const PlayCurrentPokemon_;
	PokemonBattleState* const PoeCurrentPokemon_;

	const BattleOrderMenu Select_;
	BattlePage CurrentBattlePage_;
	Battlefont CurrentFont_;

	void BattleFirstPage();

	float IsEffect(DamageType _DamgeType);

	bool Critical_;
	bool PlayerFirst_;
	bool Action_;

	//BattleManager::BattleTurn* FristTurn_;

public:
	bool Update();
};

enum class DamageType
{
	Great,
	Nomal,
	Bad,
	Nothing
};

enum class PokemonAbility
{
	Att,
	Def,
	SpAtt,
	SpDef,
	Speed,
	Accuracy,
	Evasion
};

enum class BattlePage
{
	FirstBattle,
	SecondBattle,
	End
};

enum class Battlefont
{
	None,
	Att,
	Wait,
	Effect
};

