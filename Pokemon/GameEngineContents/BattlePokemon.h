#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineRenderer.h>
#include "Pokemon.h"

// ���� :
class BattlePokemon : public GameEngineActor
{
public:
	// constrcuter destructer
	BattlePokemon();
	~BattlePokemon();

	// delete Function,
	BattlePokemon(const BattlePokemon& _Other) = delete;
	BattlePokemon(BattlePokemon&& _Other) noexcept = delete;
	BattlePokemon& operator=(const BattlePokemon& _Other) = delete;
	BattlePokemon& operator=(BattlePokemon&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update() override;

private:

	GameEngineRenderer* PlayerPokemon_;
	GameEngineRenderer* OpponentPokemon_;

	float4 PlayerPokemonPos_;
	float4 OpponentPokemonPos_;
};

