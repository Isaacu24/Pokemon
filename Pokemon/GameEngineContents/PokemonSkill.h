#pragma once

// ���� :
class PokemonSkill
{
public:
	// constrcuter destructer
	PokemonSkill();
	~PokemonSkill();

	// delete Function
	PokemonSkill(const PokemonSkill& _Other) = delete;
	PokemonSkill(PokemonSkill&& _Other) noexcept = delete;
	PokemonSkill& operator=(const PokemonSkill& _Other) = delete;
	PokemonSkill& operator=(PokemonSkill&& _Other) noexcept = delete;

protected:

private:
	int Value_; //���� ��ų Ȥ�� �� ���� ��꿡 ���� ��

};

