#pragma once
#include <GameEngine/GameEngineActor.h>
#include "UIEnum.h"

class PokemonInfo;
class GameEngineContentFont;
class PokemonSummaryMenu : public GameEngineActor
{
	enum class PokemonSummaryMenuType
	{
		PokemonInfo,
		PokemonAbility,
		PokemonSkill,
		Max
	};
public:
	PokemonSummaryMenu();
	~PokemonSummaryMenu();


	PokemonSummaryMenu(const PokemonSummaryMenu& _Other) = delete;
	PokemonSummaryMenu(const PokemonSummaryMenu&& _Other) noexcept = delete;
	PokemonSummaryMenu& operator=(const PokemonSummaryMenu& _Ohter) = delete;
	PokemonSummaryMenu& operator=(const PokemonSummaryMenu&& _Other) noexcept = delete;

	//���� �ʱ�ȭ
	void InitPokemonSummaryMenu(int _PokemonOrder);
	void GetPlayerPokemon();
private:
	std::vector<PokemonInfo*> PokemonInfoList_;

	bool IsOn_; // Init�Լ��� ����Ǿ� Update�ϰ�
	int CurrentOrder_;

protected:
	void Start() override;
	void Update() override;
	void Render() override;

private:
	//���ҽ� �ʱ�ȭ
	void InitRenderer_();
	void InitFonts_();

	//����
	void DestroyPokemonSummaryMenu();

	//fsm
	void ChangeState(PokemonSummaryMenuType _State);
	void UpdateState();

	void PokemonInfoStart();
	void PokemonInfoUpdate();

	void PokemonAbilityStart();
	void PokemonAbilityUpdate();

	void PokemonSkillStart();
	void PokemonSkillUpdate();

	PokemonSummaryMenuType CurState_;


	//������
	GameEngineRenderer* TopMenuRenderer_;
	GameEngineRenderer* PokemonFrameRenderer_;
	GameEngineRenderer* PokemonInfoRenderer_; //�̰� ž�޴� �Ʒ� ūâ
	GameEngineRenderer* PokemonSkillGreenBoxRenderer_;

	std::vector<GameEngineRenderer*> PokemonFrontRenderer_;
	std::vector< GameEngineRenderer*> PokemonTypeRenderer_;
	std::vector<GameEngineRenderer*> GenderRenderer_;
	std::vector<GameEngineRenderer*> PokemonSkillTypeRenderer_; // �÷��̾�� 1���� ���ϸ� ������ ������ ���� >> �׳� ���ͷ� ���������

	//��Ʈ
	std::vector<GameEngineContentFont*> LevelFonts_;
	std::vector< GameEngineContentFont*> FrameNameFonts_;
	std::vector< GameEngineContentFont*> NameFonts_;
	std::vector< GameEngineContentFont*> NumberFonts_;
	std::vector<GameEngineContentFont*> NatureFonts_;
	std::vector< GameEngineContentFont*> AllFonts_;
	std::vector<GameEngineContentFont*> CurHpFonts_;
	std::vector<GameEngineContentFont*> MaxHpFonts_;
	std::vector<GameEngineContentFont*> AttFonts_;
	std::vector<GameEngineContentFont*> DefFonts_;
	std::vector<GameEngineContentFont*> SpAttFonts_;
	std::vector<GameEngineContentFont*> SpDefFonts_;
	std::vector<GameEngineContentFont*> SpeedFonts_;
	std::vector<GameEngineContentFont*> CurExpFonts_;
	std::vector<GameEngineContentFont*> NextExpFonts_;
	std::vector<GameEngineContentFont*> AbilityFonts_;
	std::vector<GameEngineContentFont*> AbilityExplanationFonts_;
	std::vector<GameEngineContentFont*> SkillNameFonts_; // �÷��̾�� 1���� ���ϸ� ������ ������ ���� >> �׳� ���ͷ� ���������
	std::vector<GameEngineContentFont*> CurSkillPPFonts_; // �÷��̾�� 1���� ���ϸ� ������ ������ ���� >> �׳� ���ͷ� ���������
	std::vector<GameEngineContentFont*> MaxSkillPPFonts_; // �÷��̾�� 1���� ���ϸ� ������ ������ ���� >> �׳� ���ͷ� ���������
	GameEngineContentFont* Owner_; //���� �̸�
	GameEngineContentFont* IDNumber_;//ID ��Ʈ
	GameEngineContentFont* Item_; //������ ��Ʈ


	//ĳ���� ����Ƣ�°� ����
	float AccGravity_;
	float CurYPos_;
	bool IsJumpStart_;
	float JumpTime_;
	void PokemonJump();
	void PokemonJumpStart();
};

