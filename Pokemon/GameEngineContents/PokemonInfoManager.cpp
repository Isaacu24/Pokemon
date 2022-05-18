#include "PokemonInfoManager.h"
#include "Pokemon.h"
#include "PokemonSkill.h"
#include "Item.h"
#include <GameEngineBase/GameEngineString.h>

PokemonInfoManager* PokemonInfoManager::Inst_ = new PokemonInfoManager();

PokemonInfoManager::PokemonInfoManager() 
{
}

PokemonInfoManager::~PokemonInfoManager() 
{
	{
		std::map<std::string, Pokemon*>::iterator StartIter = AllPokemonList_.begin();
		std::map<std::string, Pokemon*>::iterator EndIter = AllPokemonList_.end();

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
		CreatePokemon("Charmander", PokemonType::FIRE, 1, 10, 10, 15, 15, 5);
		//Squirtle: ���α�
		CreatePokemon("Squirtle", PokemonType::WATER, 1, 10, 10, 15, 15, 5);
		//Bulbasaur: �̻��ؾ�
		CreatePokemon("Bulbasaur", PokemonType::GRASS, 1, 10, 10, 15, 15, 5);
		//Pidgey: ����
		CreatePokemon("Pidgey", PokemonType::FLYING, 1, 10, 10, 15, 15, 5);
		//Rattata: ����
		CreatePokemon("Rattata", PokemonType::NORMAL, 1, 10, 10, 15, 15, 5);
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

	PokemonSkill* PokemonSkill_ = FindSkill("Growl");
}

void PokemonInfoManager::Update()
{

}

Pokemon* PokemonInfoManager::FindPokemon(const std::string _Key)
{
	std::string Key = GameEngineString::ToUpperReturn(_Key);

	std::map<std::string, Pokemon*>::iterator FindIter = AllPokemonList_.find(Key);

	if (FindIter == AllPokemonList_.end())
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

Pokemon* PokemonInfoManager::CreatePokemon(const std::string _Key, PokemonType _Type, int _Lv, int _Att, int _Def, int _SpAtt, int _SpDef, int _Speed)
{
	Pokemon* NewPokemon = new Pokemon();

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

	AllPokemonList_.insert(std::make_pair(Key, NewPokemon));

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

	AllPokemonSkillList_.insert(std::make_pair(Key, NewSkill));

	return NewSkill;
}

Item* PokemonInfoManager::CreateItem(const std::string _Key, int _Value)
{
	Item* NewItem = new Item();
	std::string Key = GameEngineString::ToUpperReturn(_Key);

	NewItem->SetName(Key);
	NewItem->SetValue(_Value);

	return NewItem;
}

