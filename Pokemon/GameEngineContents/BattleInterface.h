#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineRenderer.h>
#include <GameEngineContentsCore/GameEngineContentFont.h>
#include "BattleEnum.h"
#include "Pokemon.h"

//�������� ������ �͵鵵 ��������� ĥ ���̴�
//���� ������ �ʾƵ� �ڵ����� �����Ǿ� ������ �ִ°͵�

//���� : ���� �������� ���� �������̽�

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
	void LevelChangeStart(GameEngineLevel* _PrevLevel) override;
	void LevelChangeEnd(GameEngineLevel* _NextLevel) override;

private:
	GameEngineRenderer* InterfaceImage; // ����Ʈ
	GameEngineRenderer* Select; // ����
	GameEngineRenderer* MyHPUI;
	GameEngineRenderer* EnemyHPUI;
	GameEngineRenderer* MyHP;
	GameEngineRenderer* EnemyHP;
	GameEngineRenderer* EXP;
	GameEngineRenderer* BattleCommend; // ��ų

	BattleInterface* MainInterface;

	BattleOrder CurOrder;

	float TimeCheck;


	void OrderCheck();
	void SelectOrder();

	bool PlayerEnd;
	//�̵��� Ÿ�̸�
	float BattleTimer_;

	// ������ Level
	class BattleLevel* Level_;

	//������ ��Ʈ ���
	class GameEngineContentFont* Fonts;
	GameEngineContentFont* BattleFont_;


	// UI ��Ʈ
	GameEngineContentFont* PlayerName_;
	GameEngineContentFont* PlayerLevel_;
	GameEngineContentFont* PlayerHP_;
	GameEngineContentFont* PoeName_;
	GameEngineContentFont* PoeLevel_;
	//
	bool OneTalk;

public:
	class BattleUnitRenderer* BattleUnit;
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
	GameEngineRenderer* GetEnemyHPUI() const
	{
		return EnemyHPUI;
	}
	GameEngineRenderer* GetMyHPUI() const
	{
		return MyHPUI;
	}
	GameEngineRenderer* GetMyHP() const
	{
		return MyHP;
	}
	GameEngineRenderer* GetEXP() const
	{
		return EXP;
	}
	GameEngineRenderer* GetInterfaceImage() const
	{
		return InterfaceImage;
	}
	GameEngineRenderer* GetSelect() const
	{
		return Select;
	}
	void StartTalk();

	// ������ : test
	bool BattleKey();
	void ShowUsedSkillString(const std::string& _AttPokemon, const std::string& _AttSkill);
	void ShowPoeFaintString(const std::string& _PoePokemon);
	void ShowPlayerFaintString(const std::string& _PlayerPokemon);
	void ShowSupperEffectString();
	void ShowCriticalHitString();
	void ShowNotEffective();
	void ShowFailed();
	void ShowRankUpAndDown(const std::string& _Pokemon, PokemonAbility _Ability, int _Rank);
	void ShowGetEXP(const std::string& _PlayerPokemon, int _EXP);
	void ShowChangePokemon(const std::string& _Poe, const std::string& _PoePokemon);
	void ShowLevelUp(const std::string& _PlayerPokemon, int _Lv);

	//
	std::vector<GameEngineContentFont*> AllSkillFont_;
	void ShowPokemonSkill(Pokemon* _Pokemon);
	void ShowAndCheckSkillPos();
	//

	std::string AbilityString(PokemonAbility _Ability);
	std::string RankString(int _Rank);


	// string ������ ȣ���ؼ� ������Ű�� �Լ�

	inline bool IsEndString()
	{
		return Fonts->IsEnd();
	}


private:
	int SkillUIPos_;
	PokemonSkillInfo* RandomPoeSkill(Pokemon* _Pokemon);

	void Reset();


////////////////////UI////////////////
private:
	GameEngineActor* ChildUI_;

	void UIUpdate();
};

// ������ : Test
enum class FightString
{
	Used,
	Effective,
	Critical,
	Became
};