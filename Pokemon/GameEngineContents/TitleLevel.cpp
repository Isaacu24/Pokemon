#include "TitleLevel.h"
#include "TitleBackground.h"
#include <GameEngineContentsCore/GameEngineContentFont.h>
#include <string>
#include <GameEngineBase/GameEngineInput.h>
#include "Pokemon.h"
#include "PokemonEngine.h"

TitleLevel::TitleLevel() 
	: TitleIntro_(nullptr)
	, Fonts(nullptr)
	, BgmPlayer{}
{
}

TitleLevel::~TitleLevel() 
{
}

void TitleLevel::Loading()
{
	GameEngineActor* Intro = CreateActor<TitleBackground>();
	
	TitleIntro_ = Intro->CreateRenderer("TitleImage_0.bmp");

	//////��Ʈ ��� �׽�Ʈ
	//{
	//	Fonts = CreateActor<GameEngineContentFont>();
	//	Fonts->SetPosition({ 200, 200 });
	//	Fonts->ShowString("Very Very \\ABCDEFGHIJ \\IntotheStage \\Red Is Red \\Blue Is Red! ", true);
	//	// \\ �� ���� �� �� �ٿ��
	//}
}

void TitleLevel::Update()
{
	Time_ += GameEngineTime::GetDeltaTime();

	if (0.07f <= Time_ 
		&& 384 > Index_)
	{
		Time_ = 0.f;
		++Index_;

		TitleIntro_->SetImage("TitleImage_" + std::to_string(Index_) + ".bmp");
	}

	PokemonEngine* TmpEngine = dynamic_cast<PokemonEngine*>(&GameEngine::GetInst());

	if (nullptr != TmpEngine)
	{
		if (true == GameEngineInput::GetInst()->IsDown("Z"))
		{
			TmpEngine->ChangeLevel("WorldMap");
		}
	}

	//{
	//	// ��Ʈ ����� �Ϸ�ǰ� Ű�Է� ���
	//	if (Fonts->IsWait())
	//	{
	//		// Z Ű �Է½� ���� ���� ���
	//		if (GameEngineInput::GetInst()->IsDown("Z") == true)
	//		{
	//			// ���� ���� ��� �Լ�
	//			Fonts->NextString();
	//		}
	//	}
	//	// ���� ������ ���� �� == ������ ��
	//	if (Fonts->IsEnd())
	//	{
	//		// ��� ��ȭ�� ������ �� z Ű������ ���� ��ȭ ����
	//		if (GameEngineInput::GetInst()->IsDown("Z") == true)
	//		{
	//			// ��µ� �Լ� ����
	//			Fonts->ShowString("String Is Hard\\ABCDEFGHIJ \\VeryHard \\Red Is Red? \\Blue Is Red! ");
	//		}
	//		// ��� ��ȭ�� ������ �� x Ű������ ����
	//		else if (GameEngineInput::GetInst()->IsDown("X") == true)
	//		{
	//			Fonts->EndFont();
	//		}
	//	}
	//}

}

void TitleLevel::LevelChangeStart(GameEngineLevel* _NextLevel)
{
	BgmPlayer = GameEngineSound::SoundPlayControl("Title(recorder).mp3");
	BgmPlayer.SetVolume(0.5f);
}
void TitleLevel::LevelChangeEnd(GameEngineLevel* _PrevLevel)
{
	BgmPlayer.Stop();
}