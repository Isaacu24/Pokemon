#pragma once
#include <GameEngine/GameEngineLevel.h>
#include "BattleNPCInterface.h"
#include "PlayerRed.h"
#include "BattleInterface.h"
#include "BattleEnum.h"

//�������� ������ �͵鵵 ��������� ĥ ���̴�
//���� ������ �ʾƵ� �ڵ����� �����Ǿ� ������ �ִ°͵�

//���� : 

enum class BattleTurn;
class BattleEngine;
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

	inline PlayerRed* GetRed() const
	{
		return PlayerRed_;
	}
	
	inline BattleTurn GetBattleTurn() const
	{
		return InBattle_;
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
	BattleInterface* Interface;
	BattleState BState_;
	BattleTurn InBattle_;

	void FirstBattlePage();
	void BattlePageStart();
	void SecondBattlePage();

	// ������ ����
	void ShowOpenning();
	bool OpenningEnd_;
	//
	
	// ���� ����
	void ShowEndding();
	bool EnddingEnd_;
	//
	


	// �÷��̾� �� NPC
	PlayerRed* PlayerRed_;
	Pokemon* PlayerPokemon_; // Debug
	BattleNPCInterface* Opponent_;
	//

	//������ ��Ʈ ���
	class GameEngineContentFont* Fonts;
	class BattleUnitRenderer* PlayerStopCheck;

	bool OneTalk;
};

enum class BattleTurn
{
	Off,
	Wait,
	FirstTurn,
	SecondTurn,
	BattleEnd
};
