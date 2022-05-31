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

enum class PokemonAbility;
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


	void OrderCheck();
	void SelectOrder();

	bool PlayerEnd;

	// ������ Level
	class BattleLevel* Level_;

	// Font
	GameEngineContentFont* DownFont_;

	//������ ��Ʈ ���
	class GameEngineContentFont* Fonts;
	class BattleUnitRenderer* BattleUnit;
	bool OneTalk;

public:
	void DoomChit();
	bool MoveKey();
	bool GetPlayerEnd() const
	{
		return PlayerEnd;
	}
	void SetPlayerEnd(bool Value)
	{
		PlayerEnd = Value;
	}
	GameEngineRenderer* GetEnemyHPUI()
	{
		return EnemyHPUI;
	}
	GameEngineRenderer* GetMyHPUI()
	{
		return MyHPUI;
	}
	GameEngineRenderer* GetMyHP()
	{
		return MyHP;
	}
	GameEngineRenderer* GetEXP()
	{
		return EXP;
	}
	GameEngineRenderer* GetInterfaceImage()
	{
		return InterfaceImage;
	}
	GameEngineRenderer* GetSelect()
	{
		return Select;
	}

	// ������ : test
	bool BattleKey();
	void ShowUsedSkillString(const std::string& _AttPokemon, const std::string& _AttSkill);
	void ShowPoeFaintString(const std::string& _PoePokemon);
	void ShowSupperEffectString();
	void ShowCriticalHitString();
	void ShowNotEffective();
	void ShowFailed();
	void ShowRankUpAndDown(const std::string& _Pokemon, PokemonAbility _Ability, int _Rank);
	std::string AbilityString(PokemonAbility _Ability);
	std::string RankString(int _Rank);


	// string ������ ȣ���ؼ� ������Ű�� �Լ�
	inline void SetEmptyString()
	{
		EmptyString_ = true;
	}

	inline bool IsEndString()
	{
		return DownFont_->IsEnd();
	}
private:
	bool EmptyString_;
};

// ������ : Test
enum class FightString
{
	Used,
	Effective,
	Critical,
	Became
};