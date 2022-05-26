#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineRenderer.h>
#include <GameEngineContentsCore/GameEngineContentFont.h>

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

enum class FightString;
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
	
	GameEngineRenderer* MonsterBall;
	
	BattleInterface* MainInterface;
	

	BattleOrder CurOrder;

	float TimeCheck;


	void DoomChit();
	void OrderCheck();
	void SelectOrder();

	bool PlayerEnd;

	// ������ Level
	class BattleLevel* Level_;

	// Font
	GameEngineContentFont* DownFont_;

	//������ ��Ʈ ���
	class GameEngineContentFont* Fonts;
	class BattleUnitRenderer* PlayerStopCheck;
	bool OneTalk;

public:
	bool MoveKey();

	// ������ : test
	bool BattleKey();
	void ShowUsedSkillString(const std::string& _AttPokemon, const std::string& _AttSkill);
	void ShowPoeFaintString(const std::string& _PoePokemon);
	void ShowSupperEffectString();
	void ShowCriticalHitString();
	void ShowNotEffective();

	inline bool IsEndString()
	{
		return DownFont_->IsEnd();
	}

};

// ������ : Test
enum class FightString
{
	Used,
	Effective,
	Critical,
	Became
};