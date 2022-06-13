#pragma once
#include "ContentEnum.h"
#include <GameEngineBase/GameEngineNameObject.h>

class PokemonSkillInfo : public GameEngineNameObject
{
public:
	// constrcuter destructer
	PokemonSkillInfo();
	~PokemonSkillInfo();

	// delete Function
	PokemonSkillInfo(const PokemonSkillInfo& _Other) = delete;
	PokemonSkillInfo(PokemonSkillInfo&& _Other) noexcept = delete;
	//PokemonSkillInfo& operator=(const PokemonSkillInfo& _Other) = delete;
	PokemonSkillInfo& operator=(PokemonSkillInfo&& _Other) noexcept = delete;

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

	inline int& GetPP()
	{
		return PP_;
	}

	inline void SetPP(int _PP)
	{
		PP_ = _PP;
	}

	inline int GetMaxPP()
	{
		return MaxPP_;
	}

	inline void SetMaxPP(int _PP)
	{
		MaxPP_ = _PP;
	}

	inline void SetEffect(const std::string& _Effect)
	{
		Effect_ = _Effect;
	}

	inline const std::string& GetEffect()
	{
		return Effect_;
	}

	inline void SetDesc(const std::string& _Desc)
	{ 
		Desc_ = _Desc;
	}

	inline const std::string& GetDesc()
	{
		return Desc_;
	}

	//PokemonSkillInfo& operator=(const PokemonSkillInfo& _Other)
	//{
	//	PokemonSkillInfo* NewSKill = new PokemonSkillInfo();

	//	NewSKill->SetName(_Other.GetNameConstRef());
	//	NewSKill->SetPP(_Other.PP_);
	//	NewSKill->SetValue(_Other.Value_);
	//	NewSKill->SetSkillType(_Other.SkillType_);
	//	NewSKill->SetEffect(_Other.Effect_);
	//	NewSKill->SetDesc(_Other.Desc_);
	//	NewSKill->SetType(_Other.Type_);

	//	return *NewSKill;
	//}

protected:	

private:
	int Value_; //���� ��ų Ȥ�� �� ���� ��꿡 ���� ��
	int MaxPP_; //�ִ� ���� Ƚ��
	int PP_; //���� ���� Ƚ��

	SkillType SkillType_; // ��ų ���� (����, ��� ��
	PokemonType Type_; // ��ų ��ü �Ӽ� (�븻, �� ��

	std::string Effect_;
	std::string Desc_; //��ų ȿ�� ����
};

