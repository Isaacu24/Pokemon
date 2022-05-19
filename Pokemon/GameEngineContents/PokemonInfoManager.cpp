#include "PokemonInfoManager.h"
#include "Pokemon.h"
#include "PokemonSkill.h"
#include "MonsterBall.h"
#include "Potion.h"

PokemonInfoManager* PokemonInfoManager::Inst_ = new PokemonInfoManager();

PokemonInfoManager::PokemonInfoManager() 
{
}

PokemonInfoManager::~PokemonInfoManager() 
{
	{
		std::map<std::string, PokemonInfo*>::iterator StartIter = AllPokemonInfoList_.begin();
		std::map<std::string, PokemonInfo*>::iterator EndIter = AllPokemonInfoList_.end();

		for (; StartIter != EndIter; ++StartIter)
		{
			if (nullptr != StartIter->second)
			{
				delete StartIter->second;
				StartIter->second = nullptr;
			}
		}
	}

	{
		std::map<std::string, PokemonSkill*>::iterator StartIter = AllPokemonSkillList_.begin();
		std::map<std::string, PokemonSkill*>::iterator EndIter = AllPokemonSkillList_.end();

		for (; StartIter != EndIter; ++StartIter)
		{
			if (nullptr != StartIter->second)
			{
				delete StartIter->second;
				StartIter->second = nullptr;
			}
		}
	}

	{
		std::map<std::string, Item*>::iterator StartIter = AllItemList_.begin();
		std::map<std::string, Item*>::iterator EndIter = AllItemList_.end();

		for (; StartIter != EndIter; ++StartIter)
		{
			if (nullptr != StartIter->second)
			{
				delete StartIter->second;
				StartIter->second = nullptr;
			}
		}
	}
}

void PokemonInfoManager::Reset()
{
	{
		//Charmander: ���̸�
		CreatePokemonInfo("Charmander", PokemonType::FIRE, 1, 10, 10, 15, 15, 5);
		//Squirtle: ���α�
		CreatePokemonInfo("Squirtle", PokemonType::WATER, 1, 10, 10, 15, 15, 5);
		//Bulbasaur: �̻��ؾ�
		CreatePokemonInfo("Bulbasaur", PokemonType::GRASS, 1, 10, 10, 15, 15, 5);
		//Pidgey: ����
		CreatePokemonInfo("Pidgey", PokemonType::FLYING, 1, 10, 10, 15, 15, 5);
		//Rattata: ����
		CreatePokemonInfo("Rattata", PokemonType::NORMAL, 1, 10, 10, 15, 15, 5);
	}

	{
		//Tackle: �����ġ��
		CreateSkill("Tackle", 10, PokemonType::NORMAL, SkillType::Attack);
		//Tail Whip: ���� ���� 
		CreateSkill("TailWhip", 10, PokemonType::NORMAL, SkillType::Debuff);
		//Scratch: ������
		CreateSkill("Scratch", 10, PokemonType::NORMAL, SkillType::Attack);
		//Scratch: �����Ҹ� 
		CreateSkill("Growl", 10, PokemonType::NORMAL, SkillType::Debuff);
	}
		
	{
		CreateItem<MonsterBall>("MonsterBall", 10);
		CreateItem<Potion>("Potion", 10);
	}
}

void PokemonInfoManager::Update()
{

}

PokemonInfo* PokemonInfoManager::FindPokemonInfo(const std::string _Key)
{
	std::string Key = GameEngineString::ToUpperReturn(_Key);

	std::map<std::string, PokemonInfo*>::iterator FindIter = AllPokemonInfoList_.find(Key);

	if (FindIter == AllPokemonInfoList_.end())
	{
		return nullptr;
	}

	return FindIter->second;
}

PokemonSkill* PokemonInfoManager::FindSkill(std::string _Key)
{
	std::string Key = GameEngineString::ToUpperReturn(_Key);

	std::map<std::string, PokemonSkill*>::iterator FindIter = AllPokemonSkillList_.find(Key);

	if (FindIter == AllPokemonSkillList_.end())
	{
		return nullptr;
	}

	return FindIter->second;
}

Item* PokemonInfoManager::FindItem(std::string _Key)
{
	std::string Key = GameEngineString::ToUpperReturn(_Key);

	std::map<std::string, Item*>::iterator FindIter = AllItemList_.find(Key);

	if (FindIter == AllItemList_.end())
	{
		return nullptr;
	}

	return FindIter->second;
}

PokemonInfo* PokemonInfoManager::CreatePokemonInfo(const std::string _Key, PokemonType _Type, int _Lv, int _Att, int _Def, int _SpAtt, int _SpDef, int _Speed)
{
	PokemonInfo* NewPokemon = new PokemonInfo();

	std::string Key = GameEngineString::ToUpperReturn(_Key);

	NewPokemon->SetName(Key);
	NewPokemon->SetMyType(_Type);
	NewPokemon->SetStatusEffect(StatusEffect::NONE);
	NewPokemon->SetMyLevel(_Lv);
	NewPokemon->SetMyMaxLevel(100);
	NewPokemon->SetHp(100);
	NewPokemon->SetMaxHp(100);
	NewPokemon->SetExp(0);
	NewPokemon->SetMaxExp(100);
	NewPokemon->SetAtt(_Att);
	NewPokemon->SetDef(_Def);
	NewPokemon->SetSpAtt(_SpAtt);
	NewPokemon->SetSpDef(_SpDef);
	NewPokemon->SetSpeed(_Speed);
	NewPokemon->SetIsPlayer(false);
	NewPokemon->SetIsGender(false);
	NewPokemon->SetPokemonImage(_Key);

	AllPokemonInfoList_.insert(std::make_pair(Key, NewPokemon));

	return NewPokemon;
}

PokemonSkill* PokemonInfoManager::CreateSkill(const std::string _Key, int _Value, PokemonType _Type, SkillType _SkillType)
{
	PokemonSkill* NewSkill = new PokemonSkill();
	std::string Key = GameEngineString::ToUpperReturn(_Key);

	NewSkill->SetName(Key);
	NewSkill->SetValue(_Value);
	NewSkill->SetType(_Type);
	NewSkill->SetSkillType(_SkillType);
	//NewSkill->SetMyEffect(NewSkill->CreateRenderer("Tackle.bmp"));

	AllPokemonSkillList_.insert(std::make_pair(Key, NewSkill));

	return NewSkill;
}

