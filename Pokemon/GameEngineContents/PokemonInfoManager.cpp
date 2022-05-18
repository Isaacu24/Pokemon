#include "PokemonInfoManager.h"
#include "Pokemon.h"

PokemonInfoManager* PokemonInfoManager::Inst_ = new PokemonInfoManager();

PokemonInfoManager::PokemonInfoManager() 
{
}

PokemonInfoManager::~PokemonInfoManager() 
{
}

void PokemonInfoManager::Reset()
{
	{
		CreatePokemon("���̸�", PokemonType::FIRE, 1, 10, 10, 15, 15, 5);
		CreatePokemon("���α�", PokemonType::WATER, 1, 10, 10, 15, 15, 5);
		CreatePokemon("�̻��ؾ�", PokemonType::GRASS, 1, 10, 10, 15, 15, 5);
		CreatePokemon("����", PokemonType::FLYING, 1, 10, 10, 15, 15, 5);
		CreatePokemon("����", PokemonType::NORMAL, 1, 10, 10, 15, 15, 5);
	}
}

void PokemonInfoManager::Update()
{
}

Pokemon* PokemonInfoManager::CreatePokemon(std::string _Key, PokemonType _Type, int _Lv, int _Att, int _Def, int _SpAtt, int _SpDef, int _Speed)
{
	Pokemon* NewPokemon = new Pokemon();
	NewPokemon->SetName(_Key);
	NewPokemon->SetMyType(_Type);
	NewPokemon->SetStatusEffect(StatusEffect::NONE);
	NewPokemon->SetMyLevel(_Lv);
	NewPokemon->SetAtt(_Att);
	NewPokemon->SetDef(_Def);
	NewPokemon->SetSpAtt(_SpAtt);
	NewPokemon->SetSpDef(_SpDef);
	NewPokemon->SetSpeed(_Speed);

	AllPokemonList_.insert(std::make_pair(_Key, NewPokemon));

	return NewPokemon;
}

PokemonSkill* PokemonInfoManager::CreateSkill()
{
	return nullptr;
}

Item* PokemonInfoManager::CreateItem()
{
	return nullptr;
}

