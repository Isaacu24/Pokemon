#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineRenderer.h>

//�������� ������ �͵鵵 ��������� ĥ ���̴�
//���� ������ �ʾƵ� �ڵ����� �����Ǿ� ������ �ִ°͵�

//���� : ���� �������� ���� �������̽�
class GameEngineImage;
class BattleInerface : public GameEngineActor
{
public:
	//����Ʈ ������
	BattleInerface();
	//����Ʈ �Ҹ���
	~BattleInerface();
	
	
	//======�Ʒ��͵��� ��������� �Ⱦ��ڽ��ϴ�(delete)======
	
	//����Ʈ ���� ������
	BattleInerface(const BattleInerface& _Other) = delete;
	//RValue Reference ������ (���߿� ����)
	BattleInerface(BattleInerface&& _Other) noexcept = delete;
	//operater= (�ڱ��ڽ��� �����ϴ�)
	BattleInerface& operator=(const BattleInerface& _Other) = delete;
	BattleInerface& operator=(BattleInerface&& _Other) noexcept = delete;


	GameEngineRenderer* Select;


protected:
	GameEngineRenderer* InterfaceImage;

private:

	BattleInerface* MainInterface;

public:
	void Start() override;
	void Render() override;
	void Update() override;

};

