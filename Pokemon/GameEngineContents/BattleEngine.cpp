#include "BattleEngine.h"
#include "BattleLevel.h"
#include "PokemonInfoManager.h"
#include <GameEngineBase/GameEngineDebug.h>

BattleEngine::BattleEngine() 
{
}

BattleEngine::~BattleEngine() 
{
}

void BattleEngine::Start()
{
	// Opponent
}

void BattleEngine::Update()
{

}

void BattlePage(Pokemon* _PlayerPokemon, Pokemon* _OpponentPokemon, const std::string& _PlayerSkill, const std::string& _OpponentSkill)
{
	// ��ų�� �ش� ���ϸ��� ��ų ������ �ִ��� üũ
	_PlayerPokemon->GetInfo()->GetSkill();
	PokemonSkill* PlayerSkill = PokemonInfoManager::GetInst().FindSkill("_PlayerSkill");
	if (PlayerSkill == nullptr)
	{
		MsgBoxAssert("��ų ���� �ٸ��ϴ�")
	}
	PokemonSkill* OpponentSkill = PokemonInfoManager::GetInst().FindSkill("_OpponentSkill");
	if (PlayerSkill == nullptr)
	{
		MsgBoxAssert("��ų ���� �ٸ��ϴ�")
	}



	//_PlayerPokemon->GetInfo()->
}