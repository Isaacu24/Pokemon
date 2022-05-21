#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineRenderer.h>

//�������� ������ �͵鵵 ��������� ĥ ���̴�
//���� ������ �ʾƵ� �ڵ����� �����Ǿ� ������ �ִ°͵�

//���� : ���� �������� ���� �������̽�

enum class BattleOrder
{
	Fight,
	Pokemon,
	Bag,
	Run,
	None
};

class GameEngineImage;
class BattleInterface : public GameEngineActor
{
public:
	//����Ʈ ������
	BattleInterface();
	//����Ʈ �Ҹ���
	~BattleInterface();
	
	
	//======�Ʒ��͵��� ��������� �Ⱦ��ڽ��ϴ�(delete)======
	
	//����Ʈ ���� ������
	BattleInterface(const BattleInterface& _Other) = delete;
	//RValue Reference ������ (���߿� ����)
	BattleInterface(BattleInterface&& _Other) noexcept = delete;
	//operater= (�ڱ��ڽ��� �����ϴ�)
	BattleInterface& operator=(const BattleInterface& _Other) = delete;
	BattleInterface& operator=(BattleInterface&& _Other) noexcept = delete;



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
	void Start() override;
	void Render() override;
	void Update() override;

private:
	GameEngineRenderer* InterfaceImage;
	GameEngineRenderer* Select;
	GameEngineRenderer* MyHPUI;
	GameEngineRenderer* EnemyHPUI;
	GameEngineRenderer* MyHP;
	GameEngineRenderer* EnemyHP;
	GameEngineRenderer* EXP;
	GameEngineRenderer* BattleCommend;
	

	BattleInterface* MainInterface;

	BattleOrder CurOrder;

	float TimeCheck;


	void DoomChit();
	void OrderCheck();
	void SelectOrder();
	class BattleLevel* Level_;

public:
	bool MoveKey();
	void FirstBattlePage();
};

