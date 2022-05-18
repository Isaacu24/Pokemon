#pragma once
#include <string>
#include <vector>
#include "ContentEnum.h"
#include <GameEngine/GameEngineActor.h>

class PokemonSkill;
class Pokemon : public GameEngineActor
{
public:
	Pokemon();
	~Pokemon();

	Pokemon(const Pokemon& _Other) = delete;
	Pokemon(Pokemon&& _Other) noexcept = delete;
	Pokemon& operator=(const Pokemon& _Other) = delete;
	Pokemon& operator=(Pokemon&& _Other) noexcept = delete;


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

	inline StatusEffect SetStatusEffect()
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

	inline int GetMyMaxLevel(int _MaxLv)
	{
		return MaxLv_;
	}

	inline void SetMyMaxLevel(int _MaxLv)
	{
		MaxLv_ = _MaxLv;
	}

	inline int GetHp()
	{
		return Hp_;
	}

	inline void SetHp(int _Hp)
	{
		Hp_ = _Hp;
	}

	inline int GetMaxHp()
	{
		return MaxHp_;
	}

	inline void SetMaxHp(int _MaxHp)
	{
		MaxHp_ = _MaxHp;
	}

	inline int GetExp()
	{
		return Exp_;
	}

	inline void SetExp(int _Exp)
	{
		Exp_ = _Exp;
	}

	inline int GetMaxExp()
	{
		return MaxExp_;
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
	inline GameEngineRenderer* GetMyBattleBack()
	{
		return BattleBack_;
	}

	inline void SetMyBattleBack(GameEngineRenderer* _Image)
	{
		BattleBack_ = _Image;
	}

	inline GameEngineRenderer* GetMyBattleFront()
	{
		return BattleFront_;
	}

	inline void SetMyBattleFront(GameEngineRenderer* _Image)
	{
		BattleFront_ = _Image;
	}

	inline GameEngineRenderer* GetMyMyProfile()
	{
		return MyProfile_;
	}

	inline void SetMyMyProfile(GameEngineRenderer* _Image)
	{
		MyProfile_ = _Image;
	}

	inline GameEngineRenderer* GetMyIcon()
	{
		return MyIcon_;
	}

	inline void SetMyIcon(GameEngineRenderer* _Image)
	{
		MyIcon_ = _Image;
	}

protected:
	void Start() override;
	void Update() override;

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

	GameEngineRenderer* BattleBack_; //���� �޸��(�÷��̾� ���ϸ�
	GameEngineRenderer* BattleFront_; //���� �ո��(�� Ȥ�� �߻� ���ϸ�
	GameEngineRenderer* MyProfile_; //���� Ȥ�� �󼼸޴� �̹���
	GameEngineRenderer* MyIcon_; //���� ���ϸ� ������

	std::vector<PokemonSkill*> MySkills_;
};

