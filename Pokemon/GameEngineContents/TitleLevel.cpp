#include "TitleLevel.h"
#include "TitleBackground.h"
#include <GameEngineContentsCore/GameEngineContentFont.h>
#include <string>
#include <GameEngineBase/GameEngineInput.h>

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
		Fonts->SetPosition({ 200, 200 });
		// \\ �� ���� �� �� �ٿ��
		Fonts->ShowString("Very Very \\ ABCDEFGHIJ \\ IntotheStage \\ Red Is Red \\ Blue Is Red! ");
	}
}

void TitleLevel::Update()
{

	{
		// ��Ʈ ����� �Ϸ�ǰ� Ű�Է� ���
		if (Fonts->IsWait())
		{
			// Z Ű �Է½� ���� ���� ���
			if (GameEngineInput::GetInst()->IsDown("Z") == true)
			{
				// ���� ���� ��� �Լ�
				Fonts->NextString();
			}
			// ���� ������ ���� �� == ������ ��
			if (Fonts->IsEnd())
			{
				int a = 0;
			}
		}
	}

}