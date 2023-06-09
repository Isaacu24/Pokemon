#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineRenderer.h>
#include <GameEngineContentsCore/GameEngineContentFont.h>
#include "BattleEnum.h"
#include "Pokemon.h"

//선생님은 생략된 것들도 명시적으로 칠 것이다
//직접 만들지 않아도 자동으로 생략되어 생성되 있는것들

//설명 : 전투 선택지를 띄우는 인터페이스

enum class PokemonAbility;
enum class FightString;
class GameEngineImage;
class BattleInterface : public GameEngineActor
{
	friend class BattleUnitRenderer;
public:
	//디폴트 생성자
	BattleInterface();
	//디폴트 소멸자
	~BattleInterface();


	//======아래것들은 명시적으로 안쓰겠습니다(delete)======

	//디폴트 복사 생성자
	BattleInterface(const BattleInterface& _Other) = delete;
	//RValue Reference 생성자 (나중에 배울것)
	BattleInterface(BattleInterface&& _Other) noexcept = delete;
	//operater= (자기자신을 리턴하는)
	BattleInterface& operator=(const BattleInterface& _Other) = delete;
	BattleInterface& operator=(BattleInterface&& _Other) noexcept = delete;


protected:
	void Start() override;
	void Render() override;
	void Update() override;
	void LevelChangeStart(GameEngineLevel* _PrevLevel) override;
	void LevelChangeEnd(GameEngineLevel* _NextLevel) override;

private:
	GameEngineRenderer* InterfaceImage; // 파이트
	GameEngineRenderer* Select; // 선택
	GameEngineRenderer* MyHPUI;
	GameEngineRenderer* EnemyHPUI;
	GameEngineRenderer* MyHP;
	GameEngineRenderer* EnemyHP;
	GameEngineRenderer* EXP;
	GameEngineRenderer* BattleCommend; // 스킬


	BattleInterface* MainInterface;

	BattleOrder CurOrder;

	float TimeCheck;


	void OrderCheck();
	void SelectOrder();

	bool PlayerEnd;
	//이동원 타이머
	float BattleTimer_;

	// 생성한 Level
	class BattleLevel* Level_;

	//전투시 폰트 출력
	class GameEngineContentFont* Fonts;
	GameEngineContentFont* BattleFont_;


	// UI 폰트
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
	GameEngineRenderer* GetEnemyHP() const
	{
		return EnemyHP;
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

	void StartWildTalk();
	void StartNPCTalk();
	//김예나 : test
	void NextNPCTalk();

	// 장중혁 : test
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
	void ShowLevelUp(const std::string& _PlayerPokemon, int _Lv, bool _Idx = false);
	void ShowRunaway();
	void ShowUsePokeball();
	void ShowGotPokemonByBall(const std::string& _WildPokemon);

	//
	std::vector<GameEngineContentFont*> AllSkillFont_;
	void ShowPokemonSkill(Pokemon* _Pokemon);
	void ShowAndCheckSkillPos();


	// 레벨업 폰트 밑 함수
	void LevelStatu();
	std::vector<GameEngineContentFont*> AllLevelUpFont_;
	GameEngineRenderer* BattleStatus_;
	//

	std::string AbilityString(PokemonAbility _Ability);
	std::string RankString(int _Rank);


	// string 없을때 호출해서 생략시키는 함수

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
	void ShowSkillInfo(int _Index);

	void HPChangeAnimation();
	void HPRenderUpdate(); //HP바의 렌더정보를 업데이트 하는 함수
	float HpRenderTimer_;//HP쭉 다는거 타이머
	float ExpRenderTimer_;

	int PrevFoeHp_; //적이 받은 데미지
	int PrevPlayerHp_; //  플레이어가 받은 데미지
	int PrevExp_;

	float LerpFoeHp_;//적의 HP보간값을 저장할 변수
	float LerpPlayerHp_; //플레이어의 HP보간값을 저장할 변수
	float LerpExp_; //Exp 보간값을 저장할 변수

	GameEngineContentFont* PPFont_;
	GameEngineContentFont* MaxPPFont_;

	GameEngineContentFont* TypeFont_;

public:



};

// 장중혁 : Test
enum class FightString
{
	Used,
	Effective,
	Critical,
	Became
};