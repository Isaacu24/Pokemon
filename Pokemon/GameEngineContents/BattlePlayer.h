#pragma once

//�������� ������ �͵鵵 ��������� ĥ ���̴�
//���� ������ �ʾƵ� �ڵ����� �����Ǿ� ������ �ִ°͵�

//���� : 
class BattlePlayer
{
public:
	//����Ʈ ������
	BattlePlayer();
	//����Ʈ �Ҹ���
	~BattlePlayer();

	
	
	
	//======�Ʒ��͵��� ��������� �Ⱦ��ڽ��ϴ�(delete)======
	
	//����Ʈ ���� ������
	BattlePlayer(const BattlePlayer& _Other) = delete;
	//RValue Reference ������ (���߿� ����)
	BattlePlayer(BattlePlayer&& _Other) noexcept = delete;
	//operater= (�ڱ��ڽ��� �����ϴ�)
	BattlePlayer& operator=(const BattlePlayer& _Other) = delete;
	BattlePlayer& operator=(BattlePlayer&& _Other) noexcept = delete;


protected:

private:

};

