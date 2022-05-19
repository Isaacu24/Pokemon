#pragma once
#include <string>
#include <vector>
#include "ContentEnum.h"
#include <GameEngine/GameEngineImage.h>
#include <GameEngineBase/GameEngineNameObject.h>

class PokemonSkill;
class PokemonInfo : public GameEngineNameObject
{
public:
	PokemonInfo();
	~PokemonInfo();

	PokemonInfo(const PokemonInfo& _Other) = delete;
	PokemonInfo(PokemonInfo&& _Other) noexcept = delete;
	PokemonInfo& operator=(const PokemonInfo& _Other) = delete;
	PokemonInfo& operator=(PokemonInfo&& _Other) noexcept = delete;


	//Ÿ�� �� ���� ���� �Լ�
	inline void SetMyType(PokemonType _Type)
	{
		Type_ = _Type;
	}

	inline PokemonType GetMyType()
	{
		return Type_;
	}	

	inline void SetStatusEffect(StatusEffect _Status)
	{
		Effect_ = _Status;
	}

	inline StatusEffect GetStatusEffect()
	{
		return Effect_;
	}



	//�ɷ�ġ ���� �Լ�
	inline int GetMyLevel()
	{
		return Lv_;
	}

	inline void SetMyLevel(int _Lv)
	{
		Lv_ = _Lv;
	}

	inline int GetMyMaxLevel()
	{
		return MaxLv_;
	}

	inline void SetMyMaxLevel(int _MaxLv)
	{
		MaxLv_ = _MaxLv;
	}

	inline int& GetHp()
	{
		return Hp_;
	}

	inline void SetHp(int _Hp)
	{
		Hp_ = _Hp;
	}

	inline void PlusHp(int _Value) //Hp ����
	{
		Hp_ += _Value;
	}

	inline int& GetMaxHp()
	{
		return MaxHp_;
	}

	inline void SetMaxHp(int _MaxHp)
	{
		MaxHp_ = _MaxHp;
	}

	inline int& GetExp()
	{
		return Exp_;
	}

	inline void SetExp(int _Exp)
	{
		Exp_ = _Exp;
	}

	inline int& GetMaxExp()
	{
		return MaxExp_;
	}

	inline void PlusExp(int _Value) //Exp ����
	{
		Exp_ += _Value;
	}

	inline void SetMaxExp(int _MaxExp)
	{
		MaxExp_ = _MaxExp;
	}

	inline int GetAtt()
	{
		return Att_;
	}

	inline void SetAtt(int _Att)
	{
		Att_ = _Att;
	}

	inline int GetSpAtt()
	{
		return SpAtt_;
	}

	inline void SetSpAtt(int _SpAtt)
	{
		SpAtt_ = _SpAtt;
	}

	inline int GetDef()
	{
		return Def_;
	}

	inline void SetDef(int _Def)
	{
		Def_ = _Def;
	}

	inline int GetSpDef()
	{
		return SpDef_;
	}

	inline void SetSpDef(int _SpDef)
	{
		SpDef_ = _SpDef;
	}

	inline int GetSpeed()
	{
		return Speed_;
	}

	inline void SetSpeed(int _Speed)
	{
		Speed_ = _Speed;
	}



	//�÷��̾��� ���ϸ����� üũ
	inline bool GetIsPlayer()
	{
		return IsPlayer_;
	}

	inline void SetIsPlayer(bool _IsPlayer)
	{
		IsPlayer_ = _IsPlayer;
	}

	inline bool GetGender()
	{
		return IsGender_;
	}

	inline void SetIsGender(bool _IsGender)
	{
		IsGender_ = _IsGender;
	}



	//������ ���� �Լ�
	inline std::string& GetMyBattleBack()
	{
		return BattleBack_;
	}

	inline std::string& GetMyBattleFront()
	{
		return BattleFront_;
	}

	inline std::string& GetMyMyProfile()
	{
		return MyProfile_;
	}

	inline std::string& GetMyIcon()
	{
		return MyIcon_;
	}


	void SetPokemonImage(const std::string _Name);

	
	//��ų ���� �Լ�
	void AddMySkill(PokemonSkill* _Skill);

	std::vector<PokemonSkill*>& GetSkill()
	{
		return MySkills_;
	}

protected:

private:
	PokemonType Type_;
	StatusEffect Effect_; //�����̻�

	int Lv_;
	int MaxLv_;
	int Hp_;
	int MaxHp_;
	int Exp_;
	int MaxExp_;
	int Att_;
	int Def_;
	int SpAtt_;
	int SpDef_;
	int Speed_;

	bool IsPlayer_; //�÷��̾� ���ϸ����� �ƴ���
	bool IsGender_; //true: ����, false: ����

	std::string BattleBack_; //���� �޸��(�÷��̾� ���ϸ�
	std::string BattleFront_; //���� �ո��(�� Ȥ�� �߻� ���ϸ�
	std::string MyProfile_; //���� Ȥ�� �󼼸޴� �̹���
	std::string MyIcon_; //���� ���ϸ� ������

	std::vector<PokemonSkill*> MySkills_;
	int MySkillCount_;
};

