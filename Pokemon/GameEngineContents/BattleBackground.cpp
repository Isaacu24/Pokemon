#include "BattleBackground.h"

BattleBackground::BattleBackground()
{

}

BattleBackground::~BattleBackground()
{

}

void BattleBackground::Start()
{
	//CreateRenderer("Battle_Back_NO_Ground.bmp");
	CreateRenderer("Battle_Back.bmp");
	SetPosition({ 480.0f, 225.0f });//�̹��� ��ġ �밡�� ���۾�..
}

void BattleBackground::Update()
{
}

void BattleBackground::Render()
{
}
