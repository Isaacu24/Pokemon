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
	BackgroundRender = CreateRenderer("Battle_Back.bmp", 0, RenderPivot::CENTER, { 480.0f, 225.0f});
	//SetPosition({ 480.0f, 225.0f });//�̹��� ��ġ �밡�� ���۾�..
	TextRender = CreateRenderer("Combat_TextFrame1.bmp", 0 ,RenderPivot::CENTER, {480.0f, 548.0f});
}

void BattleBackground::Update()
{
}

void BattleBackground::Render()
{
}
