#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineRenderer.h>
#include "Pokemon.h"

// ���� : BattleLevel���� �����Ǵ� �������̽��� ������ ��� �������� ������ �ִ� Actor
class BattleUnitRenderer : public GameEngineActor
{
public:
	// constrcuter destructer
	BattleUnitRenderer();
	~BattleUnitRenderer();

	// delete Function
	BattleUnitRenderer(const BattleUnitRenderer& _Other) = delete;
	BattleUnitRenderer(BattleUnitRenderer&& _Other) noexcept = delete;
	BattleUnitRenderer& operator=(const BattleUnitRenderer& _Other) = delete;
	BattleUnitRenderer& operator=(BattleUnitRenderer&& _Other) noexcept = delete;


	static GameEngineRenderer* PlayerRenderer_;


protected:
	void Start() override;
	void Update() override;

private:
	GameEngineRenderer* PlayerCurrentPokemon_;
	GameEngineRenderer* PoeCurrentPokemon_;


	GameEngineRenderer* OpponentRenderer_;


	float4 PlayerPokemonPos_;
	float4 OpponentPokemonPos_;
	float4 PlayerRendererPos_;
	float4 OpponentRenderer_Pos_;

	float PlayerTime_;
	float MoveSpeed;

	bool PlayerStop;//�迹�� : ���� ��ġ �����ϸ� ���ߵ��� �� ���ǹ�
	bool FirstMove;//�迹�� : PlayerStop��� ������ �ʵ��� ���ǹ��� bool
	float TimeCheck;

public:
	void DoomChit();
	bool GetPlayerStop()
	{
		return PlayerStop;
	}

	GameEngineRenderer* GetPlayerCurrentPokemon()
	{
		return PlayerCurrentPokemon_;
	}
};

