#pragma once
#include <GameEngine/GameEngineActor.h>

class GameEngineContentFont;
class PokemonInfo;
class PokemonMenu : public GameEngineActor
{
	enum class PokemonMenuType
	{
		SelectPokemon,
		SelectAction,
		Max
	};
public:
	PokemonMenu();
	~PokemonMenu();


	PokemonMenu(const PokemonMenu& _Other) = delete;
	PokemonMenu(const PokemonMenu&& _Other) noexcept = delete;
	PokemonMenu& operator=(const PokemonMenu& _Ohter) = delete;
	PokemonMenu& operator=(const PokemonMenu&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update() override;
	void Render() override;

private:

	//Fsm
	void ChangeState(PokemonMenuType _Type);
	void UpdateState();

	void SelectPokemonStart();
	void SelectPokemonUpdate();
	
	void SelectActionStart();
	void SelectActionUpdate();

	PokemonMenuType CurState_;

	void InitRenderer();
	void GetPlayerPokemon();
	void OnUI(); //UI�� �������
	void ChangeHp(int _PokemonIndex, int _value);


	//��Ʈ 
	std::list<GameEngineContentFont*> AllFonts_; //���߿� OffUI�϶� ��� ��Ʈ �����ϱ�
	std::vector<GameEngineContentFont*> CurHpFonts_; //���߿� OffUi�� �� ��� ��Ʈ Ŭ�����ϱ�
	std::vector<GameEngineContentFont*> SelectFonts_;

	GameEngineContentFont* DialogFont_;
	GameEngineContentFont* QuestionFont_;
	void InitFont();

	//������
	GameEngineRenderer* BackgroundRenderer_;
	GameEngineRenderer* BoxRenderer_[6];
	GameEngineRenderer* DialogRenderer_;
	GameEngineRenderer* QuestionDialogRenderer_;
	GameEngineRenderer* SelectDialogRenderer_;
	GameEngineRenderer* CancelRenderer_;
	GameEngineRenderer* PokemonRenderer_[6];
	GameEngineRenderer* HpRenderer_[6];

	//���ϸ� ������ ����
	float CurTickTime_;
	bool IsJump_;
	void IconJump(GameEngineRenderer* _Render);
	void ResetJump();

	//----------------------------------------- UI Ű�Է� ����----------------------------------------------------
	//������ �ִ� ���ϸ� ��
	int PokemonNumber_;
	//���� Ű�� ����Ű�� �ִ� ����
	int CurrentOrder_;
	//���� ������ ��ư�� ������ �� ����ϴ� Ű ����
	int RememberOrder_;
	// ���ϸ� ����Ʈ
	std::vector<PokemonInfo*> PokemonList_;

};

