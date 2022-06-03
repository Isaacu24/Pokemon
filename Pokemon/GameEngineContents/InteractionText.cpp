#include "InteractionText.h"
#include <GameEngineContentsCore/GameEngineContentFont.h>
#include <GameEngineBase/GameEngineInput.h>
#include "PlayerRed.h"
#include "NPCBase.h"
#include "NPC6.h"
#include "ShopChoiceOption.h"
#include "CenterChoiceOption.h"

bool InteractionText::IsCenterMove_ = false;
bool InteractionText::IsCenterAnim_ = false;
InteractionText::InteractionText() 
	: UIRenderer_(nullptr)
	, ArrowRenderer_(nullptr)
	, Fonts(nullptr)
	, IsSetting_(false)
	, IsShop_(false)
	, IsCenter_(false)
	, IsChoice_(false)
	, ZIgnore_(false)
{
}

InteractionText::~InteractionText() 
{
}

void InteractionText::AddText(const std::string& _Text)
{
	TextVector_.push_back(_Text);
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

void InteractionText::ClearText()
{
	TextVector_.clear();
	Fonts->ClearCurrentFonts();
}

void InteractionText::ChoiceEnd()
{
	IsChoice_ = false;
	IsCenter_ = false;
	IsShop_ = false;

	if (Fonts->IsWait())
	{
		Fonts->NextString();
	}
	if (Fonts->IsEnd())
	{
		PlayerRed::MainRed_->SetInteraction(false);
		Fonts->EndFont();
		NPC6::Text_ = nullptr;
		Death();
	}
}

void InteractionText::Start()
{
	
}

void InteractionText::Update()
{
	if (false == IsSetting_ || true == IsChoice_)
	{
		return;
	}
	
	std::string TmpString = Fonts->GetCurrentString();
	if (Fonts->GetCurrentString() == "May I help you?")
	{
		IsShop_ = true;
	}
	else if (Fonts->GetCurrentString() == "Is there anything else I can do?")
	{
		IsShop_ = true;
	}
	else if (Fonts->GetCurrentString() == "perfect health?")
	{
		IsCenter_ = true;
	}
	else if (Fonts->GetCurrentString() == "few seconds.")
	{
		IsCenterMove_ = true;
	}
	else if (Fonts->GetCurrentString() == "full health.")
	{
		IsCenterAnim_ = true;
		ZIgnore_ = true;
	}

	// ��Ʈ ����� �Ϸ�ǰ� Ű�Է� ���
	if (Fonts->IsWait())
	{
		MakeChoiceOption();

		// Z Ű �Է½� ���� ���� ���
		if (GameEngineInput::GetInst()->IsDown("Z") == true)
		{
			Fonts->NextString();
			IsCenterMove_ = false;
			//IsCenterAnim_ = false;
		}
	}
	// ���� ������ ���� �� == ������ ��
	if (Fonts->IsEnd())
	{
		MakeChoiceOption();
		WaitingMoveAnim();

		if (true == ZIgnore_)
		{
			return;
		}

		// ��ȭ�� ������ �� z Ű������ ����
		if (GameEngineInput::GetInst()->IsDown("Z") == true)
		{
			PlayerRed::MainRed_->SetInteraction(false);
			NPCBase::NPC_->SetNPCInteraction(false);
			IsCenterMove_ = false;
			IsCenterAnim_ = false;
			Fonts->EndFont();
			NPC6::Text_ = nullptr;
			Death();
		}
	}
}

void InteractionText::MakeChoiceOption()
{
	if (true == IsCenter_)
	{
		CenterChoiceOption* TmpOption = GetLevel()->CreateActor<CenterChoiceOption>();
		TmpOption->SetPosition(GetPosition());
		TmpOption->SetParent(this);
		IsChoice_ = true;
		return;
	}
	if (true == IsShop_)
	{
		ShopChoiceOption* TmpOption = GetLevel()->CreateActor<ShopChoiceOption>();
		TmpOption->SetPosition(GetPosition());
		TmpOption->SetParent(this);
		IsChoice_ = true;
		return;
	}
}

void InteractionText::WaitingMoveAnim()
{
	if (true == NPC6::InteractionMove_ || true == NPC6::InteractionAnim_)
	{
		return;
	}

	if (true == IsCenterMove_)
	{
		NPC6::InteractionMove_ = true;
	}
	if (true == IsCenterAnim_)
	{
		NPC6::InteractionAnim_ = true;
	}
}
