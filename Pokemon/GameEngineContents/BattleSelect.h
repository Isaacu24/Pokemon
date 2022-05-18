#pragma once
#include <GameEngine/GameEngineActor.h>

//�������� ������ �͵鵵 ��������� ĥ ���̴�
//���� ������ �ʾƵ� �ڵ����� �����Ǿ� ������ �ִ°͵�

//���� : ������ ���� ȭ��ǥ
class GameEngineImage;
class BattleSelect : public GameEngineActor
{
public:
	//����Ʈ ������
	BattleSelect();
	//����Ʈ �Ҹ���
	~BattleSelect();

	
	
	
	//======�Ʒ��͵��� ��������� �Ⱦ��ڽ��ϴ�(delete)======
	
	//����Ʈ ���� ������
	BattleSelect(const BattleSelect& _Other) = delete;
	//RValue Reference ������ (���߿� ����)
	BattleSelect(BattleSelect&& _Other) noexcept = delete;
	//operater= (�ڱ��ڽ��� �����ϴ�)
	BattleSelect& operator=(const BattleSelect& _Other) = delete;
	BattleSelect& operator=(BattleSelect&& _Other) noexcept = delete;


protected:
	GameEngineRenderer* SelectKey;

private:
	void Start() override;
	void Render() override;
	void Update() override;
};

