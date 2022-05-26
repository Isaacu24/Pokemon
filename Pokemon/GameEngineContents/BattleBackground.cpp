#include "BattleBackground.h"
#include <GameEngineBase/GameEngineTime.h>

BattleBackground::BattleBackground()
	:BackgroundRender(nullptr)
	,TextRender(nullptr)
	,BattleSelect(nullptr)
	,Fade_Up(nullptr)
	,Fade_Down(nullptr)
	,Start_Effect(nullptr)
	,Fade_Time(0.0f)


{

}

BattleBackground::~BattleBackground()
{

}

void BattleBackground::Start()
{
	//CreateRenderer("Battle_Back_NO_Ground.bmp");
	BackgroundRender = CreateRenderer("Battle_Back.bmp", 0, RenderPivot::CENTER, { 480.0f, 225.0f});
	TextRender = CreateRenderer("Combat_TextFrame1.bmp", 1 ,RenderPivot::CENTER, {480.0f, 548.0f});
	Fade_Up = CreateRenderer("FadeUp.bmp", 99, RenderPivot::CENTER, { 480.0f,320.0f });
	Fade_Down = CreateRenderer("FadeDown.bmp", 99, RenderPivot::CENTER, { 480.0f,320.0f });
	Start_Effect = CreateRenderer("StartEffect.bmp", 98, RenderPivot::CENTER, {});
	
}

void BattleBackground::Update()
{
	Fade_Time += GameEngineTime::GetDeltaTime()*200.0f;

	Fade_Up->SetPivot({ 480.0f ,320.0f - Fade_Time });
	if (Fade_Up->GetPivot().y < -640.0f)
	{	//���� ��ġ �̻� �̵��ϸ� ������(�Ⱥ����� ��� �̵����̴ϱ�..)
		Fade_Up->Off();
	}
	Fade_Down->SetPivot({ 480.0f ,320.0f + Fade_Time });
	if (Fade_Down->GetPivot().y > 640.0f)
	{	
		Fade_Down->Off();
	}
	Start_Effect->SetPivot({ 960.0f - (Fade_Time * 3),320.0f });
	Start_Effect->SetAlpha(255 - Fade_Time < 0.0f ? 0 : 255 - Fade_Time);
	//���׿����� ���İ��� 0���� ũ�� �ش簪, �ƴ϶�� 0����
	if (Start_Effect->GetPivot().x < -960.0f)
	{
		Start_Effect->Off();
	}
}

void BattleBackground::Render()
{
}
