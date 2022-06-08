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
	GameEngineRenderer* MyTackleEffect;//�迹�� : 6�� 3�� �����ġ�� ����Ʈ ������ �߰�
	GameEngineRenderer* MyWaterGunEffect;
	GameEngineRenderer* EnemyTackleEffect;

	//�������� �����ֽ� ����
	GameEngineRenderer* OpponentRenderer_;

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
	bool Fighting;//�迹�� : �ο� ä���߿� ��������

	float TimeCheck;

	float MyMoveTime;//�迹��: �� ǪŰ�� ���ݽ� Ÿ��üũ�ؼ� �� �ð��� ������� �����̰Բ�
	bool MyTurnEnd;//�迹��: �� ǪŰ�� ���ݹ����� ������ MoveTime�ʱ�ȭ��

	float Angle;//ȸ����..?
	float EffectX;//����Ʈ �ǽð�x��ġ
	float EffectY;//����Ʈ �ǽð�y��ġ

	class BattleLevel* Level_;
	class BattleData* BattleDataR_;

	void ShowDebugValue();
public:
	void Opening();
	void Opening2();
	void DoomChit();
	bool GetPlayerStop() const
	{
		return PlayerStop;
	}
	void SetPlayerStop(bool Value)
	{
		PlayerStop = Value;
	}
	//�迹�� ���� ���� �߰���(6�� 2��)
	void Tackle();
	void WaterGun();
	void TailWhipMove();
	void ShellHide();
	void SetFighting(bool Value)
	{
		Fighting = Value;
	}
};

