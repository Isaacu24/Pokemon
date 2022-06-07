#pragma once
#include <GameEngine/GameEngineActor.h>

class FadeActor;
class GameEngineContentFont;
class PokemonInfo;
class Item;
class PokemonMenu : public GameEngineActor
{
	enum class PokemonMenuType
	{
		SelectPokemon,
		SelectAction,
		SelectSwitch,
		Switching,
		OpeningChildUI,
		UsingPotion,
		Max
	};

	enum class ChildUIType
	{
		None,
		PokemonSummaryMenu,
		Max
	};
public:
	PokemonMenu();
	~PokemonMenu();


	PokemonMenu(const PokemonMenu& _Other) = delete;
	PokemonMenu(const PokemonMenu&& _Other) noexcept = delete;
	PokemonMenu& operator=(const PokemonMenu& _Ohter) = delete;
	PokemonMenu& operator=(const PokemonMenu&& _Other) noexcept = delete;

	void InitPokemonMenu(Item* _Item = nullptr);
	void DestroyPokemonMenu();

protected:
	void Start() override;
	void Update() override;
	void Render() override;

private:
	void UpdateRenderInfo();
	//Fsm
	void ChangeState(PokemonMenuType _Type);
	void UpdateState();

	void SelectPokemonStart();
	void SelectPokemonUpdate();
	
	void SelectActionStart();
	void SelectActionUpdate();

	void SelectSwitchStart();
	void SelectSwitchUpdate();

	void SwitchingStart();
	void SwitchingUpdate();

	void OpeningChildUIStart();
	void OpeningChildUIUpdate();

	void UsingPotionStart();
	void UsingPotionUpdate();

	PokemonMenuType CurState_;

	void InitRenderer();
	void GetPlayerPokemon();
	void OnUI(); //UI�� �������
	void ChangeHp(int _PokemonIndex, int _value);


	//��Ʈ 
	std::vector<GameEngineContentFont*> CurHpFonts_; //���߿� OffUi�� �� ��� ��Ʈ Ŭ�����ϱ�
	std::vector< GameEngineContentFont*> PokemonNameFonts_;
	std::vector< GameEngineContentFont*> PokemonLevelFonts_;
	std::vector< GameEngineContentFont*> MaxHpFonts_;

	GameEngineContentFont* DialogFont_;
	GameEngineContentFont* SwitchFont_;
	GameEngineContentFont* QuestionFont_;
	GameEngineContentFont* UsingPotionFont_;
	void InitFont();

	//������
	GameEngineRenderer* MenuArrowRenderer_;
	GameEngineRenderer* BackgroundRenderer_;
	GameEngineRenderer* BoxRenderer_[6];
	GameEngineRenderer* DialogRenderer_;
	GameEngineRenderer* QuestionDialogRenderer_;
	GameEngineRenderer* SelectDialogRenderer_;
	GameEngineRenderer* CancelRenderer_;
	GameEngineRenderer* UsingPotionBoxRenderer_;
	std::vector<GameEngineRenderer*> PokemonRenderer_;
	GameEngineRenderer* HpRenderer_[6];
	GameEngineRenderer* GenderRenderer_[6];

	//Switching �������
	float SwitchingTime_;
	bool IsSwitchingStart_;
	bool IsSwitchingTurn_;
	void MoveAllRenderer(int _index, float _Speed);

	//���ϸ� ������ ����
	float CurTickTime_;
	bool CanJump_[6]; //On�ϰ�� ������ �ݺ�
	void IconJump();
	void IconJumpOn(int _PokemonOrder);
	void IconJumpOff(int _PokemonOrder);

	//�ڽ� UI�� �̵� ����
	GameEngineActor* ChildUIActor_;
	ChildUIType CurChildUIType_;
	PokemonMenuType PrevMenuType_;

	//On����
	bool IsOn_; // Init�Լ��� ȣ��Ǿ� ������Ʈ�� �����ϰ� �����

	//FadeOut����
	FadeActor* FadeActor_;


	//----------------------------------------- UI Ű�Է� ����----------------------------------------------------
	//������ �ִ� ���ϸ� ��
	int PokemonNumber_;
	//���� Ű�� ����Ű�� �ִ� ���� < ���ϸ���
	int CurrentOrder_;
	//���� SelectAction���� ����
	int SelectActionOrder_;
	//���� ������ ��ư�� ������ �� ����ϴ� Ű ����
	int RememberOrder_;

	//�ٲ� ���ϸ� �ѹ�
	int ChangePokemonNumber_1;
	int ChangePokemonNumber_2;

	// ���ϸ� ����Ʈ
	std::vector<PokemonInfo*> PokemonList_;

	float MenuTime_;


	//--------------------------------------- ��ó�� �̺�Ʈ ����-------------------------------------------------------------

	bool IsItemMode_; //true : ��ó�� ��� ���, false : �Ϲ� ���
	float UsingPotionTime_;
	int PotionValue_;
};

