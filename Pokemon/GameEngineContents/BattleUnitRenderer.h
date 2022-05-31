#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineRenderer.h>
#include <stdio.h>
#include <stdlib.h>
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
	void Render() override;
	void LevelChangeStart(GameEngineLevel* _PrevLevel) override;
	void LevelChangeEnd(GameEngineLevel* _NextLevel) override;

	


private:
	GameEngineRenderer* PlayerCurrentPokemon_;
	GameEngineRenderer* PoeCurrentPokemon_;


	//* OpponentRenderer_;

	//�迹�� : 5��28�� �ű��� ������///////////////////////
	class BattleInterface* BattleInter;

	GameEngineRenderer* MonsterBall;
	float BallLerp;
	//////////////////////////////////////////////////////

	float4 PlayerPokemonPos_;
	float4 OpponentPokemonPos_;
	float4 PlayerRendererPos_;
	float4 OpponentRenderer_Pos_;

	float PlayerTime_;
	float MoveSpeed;

	bool PlayerStop;//�迹�� : ���� ��ġ �����ϸ� ���ߵ��� �� ���ǹ�
	bool FirstMove;//�迹�� : PlayerStop��� ������ �ʵ��� ���ǹ��� bool
	float TimeCheck;

	class BattleLevel* Level_;
	class BattleData* BattleDataR_;

	void ShowDebugValue();
public:
	void DoomChit();
	bool GetPlayerStop() const
	{
		return PlayerStop;
	}
	void SetPlayerStop(bool Value)
	{
		PlayerStop = Value;
	}

};

