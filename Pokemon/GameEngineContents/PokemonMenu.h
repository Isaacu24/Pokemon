#pragma once
#include <GameEngine/GameEngineActor.h>


class Pokemon;
class PokemonMenu : public GameEngineActor
{
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
	void InitRenderer();
	void GetPlayerPokemon();
	void OnUI(); //UI�� �������

	//��Ʈ 
	class GameEngineContentFont* Fonts;

	//������
	GameEngineRenderer* BackgroundRenderer_;
	GameEngineRenderer* BoxRenderer_[6];
	GameEngineRenderer* DialogRenderer_;
	GameEngineRenderer* CancelRenderer_;
	GameEngineRenderer* PokemonRenderer_[6];

	//----------------------------------------- UI Ű�Է� ����----------------------------------------------------
	//������ �ִ� ���ϸ� ��
	int PokemonNumber_;
	//���� Ű�� ����Ű�� �ִ� ����
	int CurrentOrder_;
	//���� ������ ��ư�� ������ �� ����ϴ� Ű ����
	int RememberOrder_;
	//�ӽ� ���ϸ� ����Ʈ
	std::vector<Pokemon*> PokemonList_;
};

