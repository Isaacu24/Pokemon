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



	// ��Ʈ ��� �׽�Ʈ
	//{
	//	GameEngineContentFont* Fonts = CreateActor<GameEngineContentFont>();
	//	Fonts->SetPosition({ 500, 500 });
	//	Fonts->ShowString("ABCDEFG\\ZBBABAB");
	//}
}

void TitleLevel::Update()
{
}