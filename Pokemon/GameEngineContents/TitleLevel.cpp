#include "TitleLevel.h"
#include "TitleBackground.h"
#include <GameEngineContentsCore/GameEngineContentFont.h>
#include <string>

TitleLevel::TitleLevel() 
{
}

TitleLevel::~TitleLevel() 
{
}

void TitleLevel::Loading()
{
	CreateActor<TitleBackground>();


	////��Ʈ ��� �׽�Ʈ
	{
		Fonts = CreateActor<GameEngineContentFont>();
		Fonts->SetPosition({ 500, 500 });
		Fonts->ShowString("Very Very S1212trong!?\\And Too Fast12");
	}
}

void TitleLevel::Update()
{
	
}