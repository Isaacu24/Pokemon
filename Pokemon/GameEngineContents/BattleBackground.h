#pragma once
#include <GameEngine/GameEngineActor.h>

//�������� ������ �͵鵵 ��������� ĥ ���̴�
//���� ������ �ʾƵ� �ڵ����� �����Ǿ� ������ �ִ°͵�

//���� : 
class BattleBackground : public GameEngineActor
{
public:
	//����Ʈ ������
	BattleBackground();
	//����Ʈ �Ҹ���
	~BattleBackground();

	
	
	
	//======�Ʒ��͵��� ��������� �Ⱦ��ڽ��ϴ�(delete)======
	
	//����Ʈ ���� ������
	BattleBackground(const BattleBackground& _Other) = delete;
	//RValue Reference ������ (���߿� ����)
	BattleBackground(BattleBackground&& _Other) noexcept = delete;
	//operater= (�ڱ��ڽ��� �����ϴ�)
	BattleBackground& operator=(const BattleBackground& _Other) = delete;
	BattleBackground& operator=(BattleBackground&& _Other) noexcept = delete;


protected:
	GameEngineRenderer* BackgroundRender;
	GameEngineRenderer* TextRender;
	GameEngineRenderer* BattleSelect;

private:
	void Start() override;
		// ���������� ������ ����ɶ� ȣ��ȴ�.
	void Update() override;
		// ���������� ������ ����ɶ� ȣ��ȴ�.
	void Render() override;

};

