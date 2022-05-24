#include "InteractionText.h"
#include <GameEngineContentsCore/GameEngineContentFont.h>
#include <GameEngineBase/GameEngineInput.h>
#include "PlayerRed.h"

InteractionText::InteractionText() 
	: UIRenderer_(nullptr)
	, ArrowRenderer_(nullptr)
	, Fonts(nullptr)
	, IsSetting_(false)
{
}

InteractionText::~InteractionText() 
{
}

void InteractionText::Setting()
{
	UIRenderer_ = CreateRenderer("Npc_TextFrame.bmp");
	UIRenderer_->SetPivot({ 0, 225 });

	ArrowRenderer_ = CreateRenderer("BottomArrow.bmp");
	ArrowRenderer_->SetPivot({ 0, 225 });

	std::string TmpString = "";
	for (int i = 0; i < TextVector_.size(); i++)
	{
		TmpString += TextVector_[i].c_str();
		TmpString += "\\";
	}
	Fonts = GetLevel()->CreateActor<GameEngineContentFont>();
	Fonts->SetPosition(GetPosition() + float4(-420, 160));
	Fonts->ShowString(TmpString, false);
	IsSetting_ = true;	
}

void InteractionText::Start()
{
	
}

void InteractionText::Update()
{
	if (false == IsSetting_)
	{
		return;
	}

	// ��Ʈ ����� �Ϸ�ǰ� Ű�Է� ���
	if (Fonts->IsWait())
	{
		// Z Ű �Է½� ���� ���� ���
		if (GameEngineInput::GetInst()->IsDown("Z") == true)
		{
			Fonts->NextString();
		}
	}
	// ���� ������ ���� �� == ������ ��
	if (Fonts->IsEnd())
	{
		// ��ȭ�� ������ �� z Ű������ ����
		if (GameEngineInput::GetInst()->IsDown("Z") == true)
		{
			PlayerRed::MainRed_->SetInteraction(false);
			Fonts->EndFont();
			Death();
		}
	}
}

void InteractionText::AddText(const std::string& _Text)
{
	TextVector_.push_back(_Text);
}
