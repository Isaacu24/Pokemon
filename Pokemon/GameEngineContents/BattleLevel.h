#pragma once
#include <GameEngine/GameEngineLevel.h>
#include "BattleNPCInterface.h"
#include "PlayerRed.h"
#include "BattleInterface.h"
#include "BattleEnum.h"
#include "Pokemon.h"

//�������� ������ �͵鵵 ��������� ĥ ���̴�
//���� ������ �ʾƵ� �ڵ����� �����Ǿ� ������ �ִ°͵�

//���� : 

enum class PokemonAbility;
class BattleEngine;
class PokemonBattleState;
class BattleLevel : public GameEngineLevel
{
	friend class BattleInterface;
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

	//inline PlayerRed* GetRed() const
	//{
	//	return PlayerRed_;
	//}

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
	Pokemon* PlayerCurrentPokemon_; // Debug
	Pokemon* PoeCurrentPokemon_;// Debug
	//

	
};

class BattleData
{
	friend BattleLevel;
private:
	BattleData(PlayerRed* _Player, BattleNPCInterface* _Poe, BattleLevel* _Level);
	BattleData(PlayerRed* _Player, Pokemon* _WildPokemon, BattleLevel* _Level);
	~BattleData();

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

	float GetRank(const PokemonAbility& _State);


	// �� �� ���� �������� Ȯ���ϴ� �Լ� �ʿ�
	bool SetSkill(PokemonBattleState* _AlppyPokemon, PokemonSkill* _Skill);
	void Update();

private:
	// ������ ���ϸ�
	const Pokemon* Pokemon_;

	// �ൿ ���ɿ���
	bool CanAction_;

	//���� ��ũ ����
	std::map<PokemonAbility, int> CurrentRank_;

	// PokemonSkill ��ӿ� virtual �Լ� Ing, End �ʿ�
	std::list<PokemonBattleState::ApplySkill*> AllCurrentApplySkill_;

	// ���� ����޴� ��ų��
	class ApplySkill
	{
	private:
		ApplySkill();
	public:
		ApplySkill(PokemonBattleState* _CastPokemon, PokemonSkill* _Skill)
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

		void Update() const
		{
			//Skill_->;
		}
	private:
		const PokemonBattleState* CastPokemon_;
		const PokemonSkill* Skill_;
		int LeftTurn_;
	};


};


class BattleManager
{
public:
	BattleManager();
	~BattleManager();

	BattleManager(const BattleManager& _Other) = delete;
	BattleManager(BattleManager&& _Other) noexcept = delete;
	BattleManager& operator=(const BattleManager& _Other) = delete;
	BattleManager& operator=(BattleManager&& _Other) noexcept = delete;


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
