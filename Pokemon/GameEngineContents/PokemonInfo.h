#pragma once
#include <string>
#include <vector>
#include "ContentEnum.h"
#include <GameEngine/GameEngineImage.h>
#include <GameEngineBase/GameEngineNameObject.h>
#include "PokemonSkillInfo.h"
#include "PokemonSkill.h"

class PokemonInfo : public GameEngineNameObject
{
public:
	PokemonInfo();
	~PokemonInfo();

	PokemonInfo(const PokemonInfo& _Other) = delete;
	PokemonInfo(PokemonInfo&& _Other) noexcept = delete;
	PokemonInfo& operator=(const PokemonInfo& _Other) = delete;
	PokemonInfo& operator=(PokemonInfo&& _Other) noexcept = delete;


	//타입 및 상태 관련 함수
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


	inline void SetMyId(int _id)
	{
		Id_ = _id;
	}

	inline int GetMyId()
	{
		return Id_;
	}

	//능력치 관련 함수
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

	inline void PlusHp(int _Value) //Hp 증가
	{
		Hp_ += _Value;
		//HP 최대치, 최소치 조절
		if (Hp_ <= 0)
		{
			Hp_ = 0;
		}

		if (Hp_ >= MaxHp_)
		{
			Hp_ = MaxHp_;
		}
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

	inline void PlusExp(int _Value) //Exp 증가
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

	inline void SetNature(const std::string& _Nature)
	{
		PokemonNature_ = _Nature;
	}

	inline const std::string& GetNature()
	{
		return PokemonNature_;
	}



	//플레이어의 포켓몬인지 체크
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

	inline void SetGender(bool _Gender)
	{
		IsGender_ = _Gender;
	}

	inline bool GetFaint()
	{
		return IsFaint_;
	}

	inline void SetFaint(bool _Faint)
	{
		IsFaint_ = _Faint;
	}


	//렌더러 관련 함수
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

	
	//스킬 관련 함수
	void AddMySkill(PokemonSkill* _Skill);

	void changeMySkill(int _Index, PokemonSkill* _Skill);

	//inline void SetSkill(std::vector<PokemonSkill*>& _Skills)
	//{
	//	for (int i = 0; i < _Skills.size(); ++i)
	//	{
	//		if (nullptr != _Skills[i])
	//		{
	//			PokemonSkill* Skill = new PokemonSkill();
	//			MySkills_.push_back(Skill);
	//		}
	//	}
	//}

	inline std::vector<PokemonSkill*>& GetSkill()
	{
		return MySkills_;
	}
	
	inline void SetMyItem(class Item* _Item)
	{
		MyItem_ = _Item;
	}
	
	inline Item* GetMyItem()
	{
		return MyItem_;
	}

protected:

private:
	PokemonType Type_;
	StatusEffect Effect_; //상태이상

	int Id_;
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

	bool IsFaint_; //포켓몬 기절 체크
	bool IsPlayer_; //플레이어 포켓몬인지 아닌지
	bool IsGender_; //true: 수컷, false: 암컷

	std::string PokemonNature_;
	std::string BattleBack_; //전투 뒷모습(플레이어 포켓몬
	std::string BattleFront_; //전투 앞모습(적 혹은 야생 포켓몬
	std::string MyProfile_; //도감 혹은 상세메뉴 이미지
	std::string MyIcon_; //지닌 포켓몬 아이콘

	std::vector<PokemonSkill*> MySkills_;

	Item* MyItem_;
};

