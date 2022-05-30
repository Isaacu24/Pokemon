#include "InteractionText.h"
#include <GameEngineContentsCore/GameEngineContentFont.h>
#include <GameEngineBase/GameEngineInput.h>
#include "PlayerRed.h"
#include "NPCBase.h"
#include "ShopChoiceOption.h"
#include "CenterChoiceOption.h"

InteractionText::InteractionText() 
	: UIRenderer_(nullptr)
	, ArrowRenderer_(nullptr)
	, Fonts(nullptr)
	, IsSetting_(false)
	, IsShop_(false)
	, IsCenter_(false)
	, IsChoice_(false)
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

	// 폰트 출력이 완료되고 키입력 대기
	if (Fonts->IsWait())
	{
		MakeChoiceOption();

		// Z 키 입력시 다음 문장 출력
		if (GameEngineInput::GetInst()->IsDown("Z") == true)
		{
			Fonts->NextString();
		}
	}
	// 다음 문장이 없을 때 == 끝났을 때
	if (Fonts->IsEnd())
	{
		MakeChoiceOption();

		// 대화가 끝났을 때 z 키누르면 종료
		if (GameEngineInput::GetInst()->IsDown("Z") == true)
		{
			PlayerRed::MainRed_->SetInteraction(false);
			NPCBase::NPC_->SetNPCInteraction(false);
			Fonts->EndFont();
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