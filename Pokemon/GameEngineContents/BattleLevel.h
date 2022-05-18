#pragma once
#include <GameEngine/GameEngineLevel.h>
#include "BattleSelect.h"

//�������� ������ �͵鵵 ��������� ĥ ���̴�
//���� ������ �ʾƵ� �ڵ����� �����Ǿ� ������ �ִ°͵�

//���� : 
class BattleLevel : public GameEngineLevel
{
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


protected:
	void Loading() override;
	void Update() override;


private:
	BattleSelect* Select;
};

