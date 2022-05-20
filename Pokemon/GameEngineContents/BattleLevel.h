#pragma once
#include <GameEngine/GameEngineLevel.h>
#include "PlayerRed.h"
#include "BattleInerface.h"
#include "BattleEnum.h"

//�������� ������ �͵鵵 ��������� ĥ ���̴�
//���� ������ �ʾƵ� �ڵ����� �����Ǿ� ������ �ִ°͵�

//���� : 
class BattleEngine;
class BattleLevel : public GameEngineLevel
{
	friend BattleEngine;
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

	inline PlayerRed* GetRed()
	{
		return PlayerRed_;
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
	BattleInerface* Interface;
	BattleState BState_;

	// ������ ����
	void ShowOpenning();
	bool OpenningEnd_;

	// ���� ����
	void ShowEndding();
	bool EnddingEnd_;

	BattleEngine* BEngine_;
	// �÷��̾�
	PlayerRed* PlayerRed_;

	//������ ��Ʈ ���
	class GameEngineContentFont* Fonts;
	class BattleUnitRenderer* PlayerStopCheck;

	bool OneTalk;
};