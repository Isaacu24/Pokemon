#pragma once
#include <GameEngine/GameEngineActor.h>


// ���� : NPC���� ��ӹ޴� Battle�� Interface
class PlayerRed;
class Pokemon;
class BattleNPCInterface
{
	friend PlayerRed;
public:
	// constrcuter destructer
	BattleNPCInterface();
	virtual ~BattleNPCInterface();

	// delete Function
	BattleNPCInterface(const BattleNPCInterface& _Other) = delete;
	BattleNPCInterface(BattleNPCInterface&& _Other) noexcept = delete;
	BattleNPCInterface& operator=(const BattleNPCInterface& _Other) = delete;
	BattleNPCInterface& operator=(BattleNPCInterface&& _Other) noexcept = delete;

	inline bool IsBattleNPC() const
	{
		return BattleNPC_;
	}

	//
	// �����
	inline void PushPokemon(Pokemon* _Pokemon)
	{
		CurrentPossessionPokemons_.push_back(_Pokemon);
	}
	inline std::vector<Pokemon*>& GetPokemonList()
	{
		return CurrentPossessionPokemons_;
	}
	//
	//

	virtual GameEngineActor* GetActor()
	{
		return nullptr;
	}

protected:

	inline void SetBattleNPC(bool _Idx)
	{
		BattleNPC_ = _Idx;
	}

	//Pokemon* CurrentPossessionPokemon_;

	// ���� ������ �ִ� ���ϸ�
	std::vector<Pokemon*> CurrentPossessionPokemons_;
private:

	// ��Ʋ �� �� �ִ� NPC�ΰ�?
	bool BattleNPC_;
};

