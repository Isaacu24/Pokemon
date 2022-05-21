#pragma once
#include <GameEngine/GameEngineActor.h>


// ���� : NPC���� ��ӹ޴� Battle�� Interface
class Pokemon;
class BattleNPCInterface : public GameEngineActor
{
public:
	// constrcuter destructer
	BattleNPCInterface();
	~BattleNPCInterface();

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
		CurrentPossessionPokemon_ = _Pokemon;
	}

	inline Pokemon* GetPokemon()
	{
		return CurrentPossessionPokemon_;
	}
	//
	//

protected:
	virtual void Start() override {}
	virtual void Update() override {}

	// 
	void GetRandomPokemon();

	inline void SetBattleNPC_(bool _Idx)
	{
		BattleNPC_ = _Idx;
	}

	Pokemon* CurrentPossessionPokemon_;

	// ���� ������ �ִ� ���ϸ�
	//std::vector<Pokemon*> CurrentPossessionPokemon_;
private:

	// ��Ʋ �� �� �ִ� NPC�ΰ�?
	bool BattleNPC_;
};

