#pragma once
#include <GameEngine/GameEngineActor.h>
#include "ContentEnum.h"

class GameEngineRenderer;
class PokemonSkill : public GameEngineActor
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

	void Start() override;
	void Update() override;

	inline int GetValue()
	{
		return Value_;
	}

	inline void SetValue(int _Value)
	{
		Value_ = _Value;
	}

	inline PokemonType GetType()
	{
		return Type_;
	}

	inline void SetType(PokemonType _Type)
	{
		Type_ = _Type;
	}

	inline SkillType GetSkillType()
	{
		return SkillType_;
	}

	inline void SetSkillType(SkillType _SkillType)
	{
		SkillType_ = _SkillType;
	}


	inline GameEngineRenderer* GetMyEffect()
	{
		return MyEffect_;
	}

	inline void SetMyEffect(GameEngineRenderer* _Image)
	{
		MyEffect_ = _Image;
	}

protected:

private:
	int Value_; //���� ��ų Ȥ�� �� ���� ��꿡 ���� ��
	SkillType SkillType_; //��ų ���� (����, ��� ��
	PokemonType Type_; //��ų ��ü �Ӽ� (�븻, �� ��

	GameEngineRenderer* MyEffect_;
};

