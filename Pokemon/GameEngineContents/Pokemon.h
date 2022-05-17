#pragma once
#include <string>
#include "ContentEnum.h"
#include <GameEngine/GameEngineActor.h>

class Pokemon : public GameEngineActor
{
public:
	Pokemon();
	~Pokemon();

	Pokemon(const Pokemon& _Other) = delete;
	Pokemon(Pokemon&& _Other) noexcept = delete;
	Pokemon& operator=(const Pokemon& _Other) = delete;
	Pokemon& operator=(Pokemon&& _Other) noexcept = delete;

protected:

private:
	std::string Name_;
	PokemonType Type_;
	StatusEffect Effect_; //�����̻�

	int Lv_;
	int MaxLv_;
	int Exp_;
	int MaxExp_;
	int Att_;
	int Def_;
	int SpAtt_;
	int SpDef_;
	int Speed_;

	bool IsPlayer_;

	GameEngineRenderer* BattleBack_; //���� �޸��(�÷��̾� ���ϸ�
	GameEngineRenderer* BattleFront_; //���� �ո��(�� Ȥ�� �߻� ���ϸ�
	GameEngineRenderer* MyProfile_; //���� Ȥ�� �󼼸޴� �̹���
	GameEngineRenderer* MyIcon_; //���� ���ϸ� ������

};

