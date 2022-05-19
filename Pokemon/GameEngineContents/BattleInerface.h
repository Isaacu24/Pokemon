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



	GameEngineRenderer* GetSelect()
	{
		return Select;
	}
	GameEngineRenderer* GetEnemyHPUI()
	{
		return EnemyHPUI;
	}
	GameEngineRenderer* GetMyHPUI()
	{
		return MyHPUI;
	}

protected:

private:
	GameEngineRenderer* InterfaceImage;
	GameEngineRenderer* Select;
	GameEngineRenderer* MyHPUI;
	GameEngineRenderer* EnemyHPUI;

	BattleInerface* MainInterface;

public:
	void Start() override;
	void Render() override;
	void Update() override;

	void MoveKey();
};

