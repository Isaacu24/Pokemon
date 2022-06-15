#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineRenderer.h>
#include "BattleEnum.h"
#include <stdio.h>
#include <stdlib.h>
#include "Pokemon.h"

// ���� : BattleLevel���� �����Ǵ� �������̽��� ������ ��� �������� ������ �ִ� Actor
class BattleLevel;
class BattleUnitRenderer : public GameEngineActor
{
	friend class BattleManager;
public:
	// constrcuter destructer
	BattleUnitRenderer();
	~BattleUnitRenderer();

	// delete Function
	BattleUnitRenderer(const BattleUnitRenderer& _Other) = delete;
	BattleUnitRenderer(BattleUnitRenderer&& _Other) noexcept = delete;
	BattleUnitRenderer& operator=(const BattleUnitRenderer& _Other) = delete;
	BattleUnitRenderer& operator=(BattleUnitRenderer&& _Other) noexcept = delete;


	inline bool GetMyAniTurn() const
	{
		return MyTurnEnd;
	}
	inline bool GetPoeAniTurn() const
	{
		return EnemyTurnEnd;
	}

	

	static GameEngineRenderer* PlayerRenderer_;

	SkillName SkillName_;


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
	GameEngineRenderer* Rock1;
	GameEngineRenderer* Rock2;
	GameEngineRenderer* Rock3;
	GameEngineRenderer* Rock4;
	GameEngineRenderer* X;
	GameEngineRenderer* CatchBallOpen;
	GameEngineRenderer* MonsterBallOP;
	GameEngineRenderer* MonsterBallCH;
	GameEngineRenderer* MyGrowl1;
	GameEngineRenderer* MyGrowl2;
	GameEngineRenderer* MyGrowl3;
	GameEngineRenderer* EnemyGrowl1;
	GameEngineRenderer* EnemyGrowl2;
	GameEngineRenderer* EnemyGrowl3;

	//�������� �����ֽ� ����
	GameEngineRenderer* OpponentRenderer_;

	//�迹�� : 5��28�� �ű��� ������///////////////////////
	class BattleInterface* BattleInter;

	float BallLerp;
	//////////////////////////////////////////////////////

	float BallX;
	float BallY;
	float CatchBallTime;
	float4 CatchBallPivot;
	float Alpha_Time;
	float BallFall;
	float BallFallTime;

	float4 PlayerPokemonPos_;
	float4 OpponentPokemonPos_;
	float4 PlayerRendererPos_;
	float4 OpponentRenderer_Pos_;

	float PlayerTime_;
	float PlayerTime_2;
	float MoveSpeed;
	float RockSpeed;

	bool PlayerStop;//�迹�� : ���� ��ġ �����ϸ� ���ߵ��� �� ���ǹ�
	bool FirstMove;//�迹�� : PlayerStop��� ������ �ʵ��� ���ǹ��� bool
	bool Fighting;//�迹�� : �ο� ä���߿� ��������
	bool FallCheck;//���ͺ� ������ �ٴڿ� ��Ҵ���
	bool IsCatch;//���������� ������ ������(ǪŰ�� ���� ���¿� ���� On, Off��)

	float TimeCheck;

	float MyMoveTime;//�迹��: �� ǪŰ�� ���ݽ� Ÿ��üũ�ؼ� �� �ð��� ������� �����̰Բ�
	bool MyTurnEnd;//�迹��: �� ǪŰ�� ���ݹ����� ������ MoveTime�ʱ�ȭ��
	bool MyCatchEnd;
	bool EnemyTurnEnd;

	float Angle;//ȸ����..?
	float EffectX;//����Ʈ �ǽð�x��ġ
	float EffectY;//����Ʈ �ǽð�y��ġ
	
	float4 Rock1Pivot;
	bool Rock1End;
	float4 Rock2Pivot;
	bool Rock2End;
	float4 Rock3Pivot;
	bool Rock3End;
	float4 Rock4Pivot;
	bool Rock4End;
	
	float AnimationEndTime;

	BattleLevel* Level_;
	class BattleData* BattleDataR_;

	void ShowDebugValue();
	bool Debug_;
public:
	void Opening();
	void Opening2();
	void TrainerOpening1();
	void TrainerOpening2();
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
	void MyGrowl();
	void EnemyGrowl();
	void SetFighting(bool Value)
	{
		Fighting = Value;
	}
	
	void EnemyRock();
	void EnemyTackle();
	void Catch();
	void NextPokemonAppear();
	GameEngineRenderer* GetPoeCurrentPokemonRender()
	{
		return PoeCurrentPokemon_;
	}
};

