#include "PokemonMenu.h"
#include "UIEnum.h"
#include <GameEngine/GameEngineRenderer.h>
#include <GameEngineBase/GameEngineInput.h>
#include "Pokemon.h"
#include <GameEngineContentsCore/GameEngineContentFont.h>
#include "PokemonInfoManager.h"
#include <GameEngine/GameEngineImageManager.h>
#include <GameEngine/GameEngine.h>
#include "PokemonSummaryMenu.h"
#include <GameEngineBase/GameEngineWindow.h>
#include "PlayerRed.h"
#include "FadeActor.h"
#include "Item.h"
#include "Potion.h"

//�������� ���� ���߿� ���ֱ�

PokemonMenu::PokemonMenu() :
	BackgroundRenderer_(nullptr),
	PokemonNumber_(0),
	DialogRenderer_(nullptr),
	CancelRenderer_(nullptr),
	CurrentOrder_(0),
	RememberOrder_(0),
	CurState_(PokemonMenuType::SelectPokemon),
	CurTickTime_(0),
	CanJump_{ false, },
	SwitchingTime_(0),
	IsSwitchingStart_(true),
	IsSwitchingTurn_(false),
	ChildUIActor_(nullptr),
	CurChildUIType_(ChildUIType::None),
	IsOn_(false),
	FadeActor_(nullptr),
	IsItemMode_(false),
	UsingPotionTime_(0.0f),
	PotionValue_(0),
	UsingPotionBoxRenderer_(nullptr)
{
	PokemonRenderer_.resize(6);
	PokemonNameFonts_.reserve(6);
	PokemonLevelFonts_.reserve(6);
}

PokemonMenu::~PokemonMenu()
{

}

void PokemonMenu::Start()
{
	//NullImage_Ani �����̽�
	GameEngineImage* NullImage = GameEngineImageManager::GetInst()->Find("NullImage_Ani.bmp");
	NullImage->Cut({ 32,32 });



	
}

void PokemonMenu::Update()
{
	if (IsOn_ == false)
	{
		return;
	}
	UpdateState();
}

void PokemonMenu::Render()
{
	if (IsOn_ == false)
	{
		return;
	}
	if (CurState_ == PokemonMenuType::Switching)
	{
		return;
	}
	UpdateRenderInfo();
	//������ ���� ������Ʈ 
	IconJump();


	//���ϸ� �̹��� ���� ������
	if (CurrentOrder_ == 0) //���õ� ���
	{
		BoxRenderer_[0]->SetPivot({ 8,72 });
		BoxRenderer_[0]->SetImage("PoketmonMenu_15.bmp"); //Ŀ�ٶ� �ڽ���
		//���°� Switch�� Swith�� �̹����� �ٲ۴�
		if (CurState_ == PokemonMenuType::SelectSwitch)
		{
			BoxRenderer_[0]->SetImage("PoketmonMenu_26.bmp");
		}
		IconJumpOn(0);
	}
	else
	{
		BoxRenderer_[0]->SetPivot({ 8,80 });
		//ü����1�� ���õ� ��� �̹��� ����
		BoxRenderer_[0]->SetImage("PoketmonMenu_14.bmp"); //Ŀ�ٶ� �ڽ�
		if (CurState_ == PokemonMenuType::SelectSwitch)
		{
			if (0 == ChangePokemonNumber_1)
			{
				BoxRenderer_[0]->SetImage("PoketmonMenu_25.bmp");
			}
		}
		IconJumpOff(0);
	}

	for (int i = 1; i < 6; i++)
	{
		if (CurrentOrder_ == i) //���õ� ���
		{
			BoxRenderer_[i]->SetPivot({ 352,static_cast<float>(-60 + 96 * i) });
			BoxRenderer_[i]->SetImage("PoketmonMenu_13.bmp"); //���� �ڽ�

			//���°� Switch�� Swith�� �̹����� �ٲ۴�
			if (CurState_ == PokemonMenuType::SelectSwitch)
			{
				BoxRenderer_[i]->SetImage("PoketmonMenu_24.bmp");
			}
			IconJumpOn(i);
		}
		else //���� �ȵ� ���
		{
			BoxRenderer_[i]->SetPivot({ 352,static_cast<float>(-56 + 96 * i) });
			BoxRenderer_[i]->SetImage("PoketmonMenu_12.bmp"); //���� �ڽ�
			//Switch������ ���
			if (CurState_ == PokemonMenuType::SelectSwitch)
			{
				if (i == ChangePokemonNumber_1)
				{
					BoxRenderer_[i]->SetImage("PoketmonMenu_23.bmp");
				}
			}
			IconJumpOff(i);
		}	
	}
	if (CurrentOrder_ == PokemonNumber_)
	{
		CancelRenderer_->SetPivot({ 732,522 });
		CancelRenderer_->SetImage("PoketmonMenu_11.bmp");
	}
	else
	{
		CancelRenderer_->SetPivot({ 732,530 });
		CancelRenderer_->SetImage("PoketmonMenu_10.bmp");
	}

	//Hp ������
	float HpRatio = static_cast<float>(PokemonList_[0]->GetHp()) / static_cast<float>(PokemonList_[0]->GetMaxHp());
	float HpXScale = GameEngineImageManager::GetInst()->Find("PoketmonMenu_Hp1.bmp")->GetScale().x * HpRatio;
	if (HpRatio > 0.5f)
	{
		HpRenderer_[0]->SetImage("PoketmonMenu_Hp1.bmp");
	}
	else if (HpRatio >= 0.2f && HpRatio <= 0.5f)
	{
		HpRenderer_[0]->SetImage("PoketmonMenu_Hp2.bmp");
	}
	else
	{
		HpRenderer_[0]->SetImage("PoketmonMenu_Hp3.bmp");
	}
	HpRenderer_[0]->SetScale({HpXScale ,HpRenderer_[0]->GetScale().y});
	for (size_t i = 1; i < PokemonNumber_; i++)
	{
		float HpRatio = static_cast<float>(PokemonList_[i]->GetHp()) / static_cast<float>(PokemonList_[i]->GetMaxHp());
		float HpXScale = GameEngineImageManager::GetInst()->Find("PoketmonMenu_Hp1.bmp")->GetScale().x * HpRatio;
		if (HpRatio > 0.5f)
		{
			HpRenderer_[i]->SetImage("PoketmonMenu_Hp1.bmp");
		}
		else if (HpRatio >= 0.2f && HpRatio <= 0.5f)
		{
			HpRenderer_[i]->SetImage("PoketmonMenu_Hp2.bmp");
		}
		else
		{
			HpRenderer_[i]->SetImage("PoketmonMenu_Hp3.bmp");
		}
		HpRenderer_[i]->SetScale({ HpXScale ,HpRenderer_[0]->GetScale().y });
	}



	//Curstate == SelectAction�϶��� ����
	if (CurState_ == PokemonMenuType::SelectAction)
	{
		switch (SelectActionOrder_)
		{
		case 0:
			MenuArrowRenderer_->SetPivot({ 610,370 });
			break;
		case 1:
			MenuArrowRenderer_->SetPivot({ 610,434 });
			break;
		case 2:
			MenuArrowRenderer_->SetPivot({ 610,498});
			break;
		case 3:
			MenuArrowRenderer_->SetPivot({ 610,562 });
			break;
		default:
			break;
		}
	}
	//LPPOINT mousePos; // ���콺 ��ǥ�� ������ ���� ����. POINT �ڷ����� ����������.
	//mousePos = new POINT;
	//GetCursorPos(mousePos); // ����ȭ�鿡���� ���콺 ��ǥ�� ������
	//ScreenToClient(nullptr, mousePos);
	//int x = mousePos->x;
	//TextOut(GameEngine::BackBufferDC(), 100, 100, std::to_string(x).c_str(), std::to_string(x).length());
}



void PokemonMenu::UpdateRenderInfo()
{
	//���ϸ� ���� ���� ������Ʈ
	{
		//���ϸ�
		{
			PokemonRenderer_[0]->SetPivot({ 65,200 });
			for (size_t i = 1; i < PokemonNumber_; i++)
			{
				PokemonRenderer_[i]->SetPivot({ 406, static_cast<float>(40 + 96 * i) });
			}
		}

		//�̸�
		{
			PokemonNameFonts_[0]->SetPosition(GetPosition()+float4 ( 75, 126 ));
			for (size_t i = 1; i < PokemonNumber_; i++)
			{
				PokemonNameFonts_[i]->SetPosition(GetPosition() +float4( 416,static_cast<float>(-60 + 96 * i) ));
			}
		}

		//����
		{
			PokemonLevelFonts_[0]->SetPosition(GetPosition()  +float4( 190,175 ));
			for (size_t i = 1; i < PokemonNumber_; i++)
			{
				PokemonLevelFonts_[i]->SetPosition(GetPosition()  + float4( 540,static_cast<float>(-16 + 96 * i) ));
			}
		}

		//���� ü��
		{
			CurHpFonts_[0]->SetPosition(GetPosition() +float4( 150,248 ));
			for (size_t i = 1; i < PokemonNumber_; i++)
			{
				CurHpFonts_[i]->SetPosition(GetPosition() + float4( 760,static_cast<float>(-16 + 96 * i) ));
			}
		}

		//�ִ� ü��
		{
			MaxHpFonts_[0]->SetPosition(GetPosition() +float4( 260,248 ));
			for (size_t i = 1; i < PokemonNumber_; i++)
			{
				MaxHpFonts_[i]->SetPosition(GetPosition() +float4( 870,static_cast<float>(-16 + 96 * i) ));
			}
		}

		//����
		{
			GenderRenderer_[0]->SetPivot({ 268,186 });
			for (size_t i = 1; i < PokemonNumber_; i++)
			{
				GenderRenderer_[i]->SetPivot({ 636,static_cast<float>(+96 * i) });
			}
		}
		//HP ������
		{		
			HpRenderer_[0]->SetPivot({ 128,236 });
			for (size_t i = 1; i < PokemonNumber_; i++)
			{
				HpRenderer_[i]->SetPivot({ 736,static_cast<float>(-24 + 96 * i) });
			}
		}

	}
}

void PokemonMenu::ChangeState(PokemonMenuType _Type)
{
	if (CurState_ == _Type)
	{
		return;
	}

	switch (_Type)
	{
	case PokemonMenu::PokemonMenuType::SelectPokemon:
		SelectPokemonStart();
		break;
	case PokemonMenu::PokemonMenuType::SelectAction:
		SelectActionStart();
		break;
	case PokemonMenu::PokemonMenuType::SelectSwitch:
		SelectSwitchStart();
		break;
	case PokemonMenu::PokemonMenuType::Switching:
		SwitchingStart();
		break;
	case PokemonMenu::PokemonMenuType::OpeningChildUI:
		OpeningChildUIStart();
		break;
	case PokemonMenu::PokemonMenuType::UsingPotion:
		UsingPotionStart();
		break;
	default:
		break;
	}
	CurState_ = _Type;
}

void PokemonMenu::UpdateState()
{
	switch (CurState_)
	{
	case PokemonMenu::PokemonMenuType::SelectPokemon:
		SelectPokemonUpdate();
		break;
	case PokemonMenu::PokemonMenuType::SelectAction:
		SelectActionUpdate();
		break;
	case PokemonMenu::PokemonMenuType::SelectSwitch:
		SelectSwitchUpdate();
		break;
	case PokemonMenu::PokemonMenuType::Switching:
		SwitchingUpdate();
		break;
	case PokemonMenu::PokemonMenuType::OpeningChildUI:
		OpeningChildUIUpdate();
		break;
	case PokemonMenu::PokemonMenuType::UsingPotion:
		UsingPotionUpdate();
		break;
	default:
		break;
	}
}

void PokemonMenu::SelectPokemonStart()
{


	DialogRenderer_->On();
	DialogFont_->On();
	CancelRenderer_->On();
	SwitchFont_->Off();
	QuestionDialogRenderer_->Off();
	SelectDialogRenderer_->Off();
	MenuArrowRenderer_->Off();




}

void PokemonMenu::SelectPokemonUpdate()
{
	if (GameEngineInput::GetInst()->IsDown("Down") == true)
	{
		if (CurrentOrder_ >= PokemonNumber_)
		{
			CurrentOrder_ = 0;
		}
		else
		{
			CurrentOrder_++;
		}
	}

	if (GameEngineInput::GetInst()->IsDown("Up") == true)
	{
		if (CurrentOrder_ <= 0)
		{
			CurrentOrder_ = PokemonNumber_;
		}
		else
		{
			CurrentOrder_--;
		}
	}

	if (GameEngineInput::GetInst()->IsDown("Left") == true)
	{
		if (CurrentOrder_ != PokemonNumber_ && CurrentOrder_ != 0)
		{
			RememberOrder_ = CurrentOrder_;
			CurrentOrder_ = 0;
		}
	}

	if (GameEngineInput::GetInst()->IsDown("Right") == true)
	{
		if (CurrentOrder_ == 0)
		{
			if (RememberOrder_ == 0)
			{
				RememberOrder_ = 1;
			}
			CurrentOrder_ = RememberOrder_;
		}
	}

	if (GameEngineInput::GetInst()->IsDown("Z") == true)
	{
		if (CurrentOrder_ < PokemonNumber_) //�Ϲ� ���ϸ� ��ư
		{
			if (IsItemMode_ == false)
			{
				ChangeState(PokemonMenuType::SelectAction);
				return;
			}
			else
			{
				ChangeState(PokemonMenuType::UsingPotion);
				return;
			}
			
		}
		else //ĵ����ư
		{
			DestroyPokemonMenu();
			return;
		}

	}

	if (GameEngineInput::GetInst()->IsDown("X") == true)
	{
		DestroyPokemonMenu();
		return;
	}
}

void PokemonMenu::SelectActionStart()
{
	DialogRenderer_->Off();
	DialogFont_->Off();
	CancelRenderer_->Off();
	SwitchFont_->Off();
	QuestionDialogRenderer_->On();
	SelectDialogRenderer_->On();
	MenuArrowRenderer_->On();
	//��Ʈ �ʱ�ȭ
	{
		QuestionFont_ = GetLevel()->CreateActor<GameEngineContentFont>(static_cast<int>(GetOrder()));
		QuestionFont_->SetPosition(GetPosition()+float4 ( 16, 540 ));
		QuestionFont_->ShowString("What do " + PokemonList_[CurrentOrder_]->GetNameCopy(), true);
	}

	SelectActionOrder_ = 0;
	MenuArrowRenderer_->SetPivot({ 610,370 });
}

void PokemonMenu::SelectActionUpdate()
{
	if (GameEngineInput::GetInst()->IsDown("X") == true)
	{
		ChangeState(PokemonMenuType::SelectPokemon);
		//������ ��Ʈ ����
		{
			QuestionFont_->ClearCurrentFonts();
			QuestionFont_ = nullptr;
		}
		//ChangeHp(0, 1);
	}

	if (GameEngineInput::GetInst()->IsDown("Z") == true)
	{
		switch (SelectActionOrder_)
		{
		case 0:
			//������ ��Ʈ ����
		{
			QuestionFont_->ClearCurrentFonts();
			QuestionFont_ = nullptr;
		}
			PrevMenuType_ = PokemonMenuType::SelectAction;
			CurChildUIType_ = ChildUIType::PokemonSummaryMenu;
			ChangeState(PokemonMenuType::OpeningChildUI);
			return;
			break;
		case 1:
			ChangePokemonNumber_1 = CurrentOrder_;
			ChangeState(PokemonMenuType::SelectSwitch);
			return;
			break;
		case 3:
			ChangeState(PokemonMenuType::SelectPokemon);
			return;
			break;

		default:
			break;
		}

		//ChangeHp(0, 1);
	}

	//�� �Ʒ� ����
	if (GameEngineInput::GetInst()->IsDown("Down") == true)
	{
		if (SelectActionOrder_ >= 3)
		{
			SelectActionOrder_ = 0;
		}
		else
		{
			SelectActionOrder_++;
		}
	}

	if (GameEngineInput::GetInst()->IsDown("Up") == true)
	{
		if (SelectActionOrder_ <= 0)
		{
			SelectActionOrder_ = 3;
		}
		else
		{
			SelectActionOrder_--;
		}
	}
}

void PokemonMenu::SelectSwitchStart()
{
	DialogRenderer_->On();
	CancelRenderer_->On();
	SwitchFont_->On();
	QuestionDialogRenderer_->Off();
	SelectDialogRenderer_->Off();
	MenuArrowRenderer_->Off();

	//������ ��Ʈ ����
	{
		QuestionFont_->ClearCurrentFonts();
	}
}

void PokemonMenu::SelectSwitchUpdate()
{
	if (GameEngineInput::GetInst()->IsDown("Down") == true)
	{
		if (CurrentOrder_ >= PokemonNumber_)
		{
			CurrentOrder_ = 0;
		}
		else
		{
			CurrentOrder_++;
		}
	}

	if (GameEngineInput::GetInst()->IsDown("Up") == true)
	{
		if (CurrentOrder_ <= 0)
		{
			CurrentOrder_ = PokemonNumber_;
		}
		else
		{
			CurrentOrder_--;
		}
	}

	if (GameEngineInput::GetInst()->IsDown("Left") == true)
	{
		if (CurrentOrder_ != PokemonNumber_ && CurrentOrder_ != 0)
		{
			RememberOrder_ = CurrentOrder_;
			CurrentOrder_ = 0;
		}
	}

	if (GameEngineInput::GetInst()->IsDown("Right") == true)
	{
		if (CurrentOrder_ == 0)
		{
			if (RememberOrder_ == 0)
			{
				RememberOrder_ = 1;
			}
			CurrentOrder_ = RememberOrder_;
		}
	}

	if (GameEngineInput::GetInst()->IsDown("Z") == true)
	{
		//���� ĵ�� ��ư ��ġ��, ChangeState
		if (CurrentOrder_ == PokemonNumber_)
		{
			ChangeState(PokemonMenuType::SelectPokemon);
			return;
		}
		else
		{
			ChangePokemonNumber_2 = CurrentOrder_;
			if (ChangePokemonNumber_1 != ChangePokemonNumber_2)
			{
				ChangeState(PokemonMenuType::Switching);
				return;
			}

		}

		//�̰� ���� ����
		ChangePokemonNumber_2 = CurrentOrder_;
		if (ChangePokemonNumber_1 != ChangePokemonNumber_2)
		{
			

		}
	}

	if (GameEngineInput::GetInst()->IsDown("X") == true)
	{
		ChangeState(PokemonMenuType::SelectPokemon);
	}
}

void PokemonMenu::SwitchingStart()
{
	SwitchingTime_ = 0;
	IsSwitchingStart_ = true;
	IsSwitchingTurn_ = false;
}

void PokemonMenu::SwitchingUpdate()
{
	float Speed = 1350.0f;
	float SwitchingTimeAcc = 0.7f;
	if (IsSwitchingStart_ == true)
	{
		SwitchingTime_ += GameEngineTime::GetDeltaTime();
	}
	else
	{
		SwitchingTime_ -= GameEngineTime::GetDeltaTime();
	}

	if (SwitchingTime_ <= 0 && IsSwitchingStart_ == false)
	{
		ChangeState(PokemonMenuType::SelectPokemon);
		return;
	}

	if (SwitchingTime_ >= 0.5f)
	{
		IsSwitchingStart_ = false;

		//�� �������� ���ϸ��� ������ �ٲ��
		if (IsSwitchingTurn_ == false)
		{
			IsSwitchingTurn_ = true;
			{
				//Info ����
				PokemonInfo* temp = PokemonList_[ChangePokemonNumber_1];
				PokemonList_[ChangePokemonNumber_1] = PokemonList_[ChangePokemonNumber_2];
				PokemonList_[ChangePokemonNumber_2] = temp;
			}

			//���ϸ� ������ ����
			{
				GameEngineRenderer* temp = PokemonRenderer_[ChangePokemonNumber_1];
				float4 Pos_1 = PokemonRenderer_[ChangePokemonNumber_1]->GetPivot();
				PokemonRenderer_[ChangePokemonNumber_1]->SetPivot(PokemonRenderer_[ChangePokemonNumber_2]->GetPivot());
				PokemonRenderer_[ChangePokemonNumber_2]->SetPivot(Pos_1);
				PokemonRenderer_[ChangePokemonNumber_1] = PokemonRenderer_[ChangePokemonNumber_2];
				PokemonRenderer_[ChangePokemonNumber_2] = temp;

			}

			//���� ������ ����
			{
				GameEngineRenderer* temp = GenderRenderer_[ChangePokemonNumber_1];
				float4 Pos_1 = GenderRenderer_[ChangePokemonNumber_1]->GetPivot();
				GenderRenderer_[ChangePokemonNumber_1]->SetPivot(GenderRenderer_[ChangePokemonNumber_2]->GetPivot());
				GenderRenderer_[ChangePokemonNumber_2]->SetPivot(Pos_1);
				GenderRenderer_[ChangePokemonNumber_1] = GenderRenderer_[ChangePokemonNumber_2];
				GenderRenderer_[ChangePokemonNumber_2] = temp;
			}

			//HP������ ����
			{
				GameEngineRenderer* temp = HpRenderer_[ChangePokemonNumber_1];
				float4 Pos_1 = HpRenderer_[ChangePokemonNumber_1]->GetPivot();
				HpRenderer_[ChangePokemonNumber_1]->SetPivot(HpRenderer_[ChangePokemonNumber_2]->GetPivot());
				HpRenderer_[ChangePokemonNumber_2]->SetPivot(Pos_1);
				HpRenderer_[ChangePokemonNumber_1] = HpRenderer_[ChangePokemonNumber_2];
				HpRenderer_[ChangePokemonNumber_2] = temp;
			}

			//Fonts����
			{
				//�̸�
				{
					auto* temp = PokemonNameFonts_[ChangePokemonNumber_1];
					float4 Pos_1 = PokemonNameFonts_[ChangePokemonNumber_1]->GetPosition();
					PokemonNameFonts_[ChangePokemonNumber_1]->SetPosition(PokemonNameFonts_[ChangePokemonNumber_2]->GetPosition());
					PokemonNameFonts_[ChangePokemonNumber_2]->SetPosition(Pos_1);
					PokemonNameFonts_[ChangePokemonNumber_1] = PokemonNameFonts_[ChangePokemonNumber_2];
					PokemonNameFonts_[ChangePokemonNumber_2] = temp;
				}

				//����
				{
					auto* temp = PokemonLevelFonts_[ChangePokemonNumber_1];
					float4 Pos_1 = PokemonLevelFonts_[ChangePokemonNumber_1]->GetPosition();
					PokemonLevelFonts_[ChangePokemonNumber_1]->SetPosition(PokemonLevelFonts_[ChangePokemonNumber_2]->GetPosition());
					PokemonLevelFonts_[ChangePokemonNumber_2]->SetPosition(Pos_1);
					PokemonLevelFonts_[ChangePokemonNumber_1] = PokemonLevelFonts_[ChangePokemonNumber_2];
					PokemonLevelFonts_[ChangePokemonNumber_2] = temp;
				}

				//���� ü��
				{
					auto* temp = CurHpFonts_[ChangePokemonNumber_1];
					float4 Pos_1 = CurHpFonts_[ChangePokemonNumber_1]->GetPosition();
					CurHpFonts_[ChangePokemonNumber_1]->SetPosition(CurHpFonts_[ChangePokemonNumber_2]->GetPosition());
					CurHpFonts_[ChangePokemonNumber_2]->SetPosition(Pos_1);
					CurHpFonts_[ChangePokemonNumber_1] = CurHpFonts_[ChangePokemonNumber_2];
					CurHpFonts_[ChangePokemonNumber_2] = temp;
				}

				//�ִ� ü��
				{
					auto* temp = MaxHpFonts_[ChangePokemonNumber_1];
					float4 Pos_1 = MaxHpFonts_[ChangePokemonNumber_1]->GetPosition();
					MaxHpFonts_[ChangePokemonNumber_1]->SetPosition(MaxHpFonts_[ChangePokemonNumber_2]->GetPosition());
					MaxHpFonts_[ChangePokemonNumber_2]->SetPosition(Pos_1);
					MaxHpFonts_[ChangePokemonNumber_1] = MaxHpFonts_[ChangePokemonNumber_2];
					MaxHpFonts_[ChangePokemonNumber_2] = temp;
				}

				//���ϸ� ��ġ ����
				{
					std::vector<Pokemon*>& Group = PlayerRed::MainRed_->GetPokemonList();
					Pokemon* a = Group[ChangePokemonNumber_1];
					Pokemon* b = Group[ChangePokemonNumber_2];
					Group[ChangePokemonNumber_1] = b;
					Group[ChangePokemonNumber_2] = a;
				}
			}

		}
	}

	if (IsSwitchingStart_ == true)
	{
		if (ChangePokemonNumber_1 == 0)
		{
			MoveAllRenderer(ChangePokemonNumber_1, Speed* SwitchingTimeAcc);
			MoveAllRenderer(ChangePokemonNumber_2, Speed);
		}
		else if (ChangePokemonNumber_2 == 0)
		{
			MoveAllRenderer(ChangePokemonNumber_2, Speed * SwitchingTimeAcc);
			MoveAllRenderer(ChangePokemonNumber_1, Speed);
		}
		else
		{
			MoveAllRenderer(ChangePokemonNumber_1, Speed);
			MoveAllRenderer(ChangePokemonNumber_2, Speed);
		}
	}
	else
	{
		if (ChangePokemonNumber_1 == 0)
		{
			MoveAllRenderer(ChangePokemonNumber_1, -Speed * SwitchingTimeAcc);
			MoveAllRenderer(ChangePokemonNumber_2,- Speed);
		}
		else if (ChangePokemonNumber_2 == 0)
		{
			MoveAllRenderer(ChangePokemonNumber_2, -Speed * SwitchingTimeAcc);
			MoveAllRenderer(ChangePokemonNumber_1, -Speed);
		}
		else
		{
			MoveAllRenderer(ChangePokemonNumber_1,- Speed);
			MoveAllRenderer(ChangePokemonNumber_2, -Speed);
		}
	}
}

void PokemonMenu::OpeningChildUIStart()
{
	if (ChildUIActor_ != nullptr)
	{
		ChildUIActor_ = nullptr;
	}

	//���ͻ���
	switch (CurChildUIType_)
	{

	case PokemonMenu::ChildUIType::PokemonSummaryMenu:
		FadeActor_->FadeOut();
		ChildUIActor_ = GetLevel()->CreateActor<PokemonSummaryMenu>(GetOrder()+10, "PokemonSummaryMenu"); 
		ChildUIActor_->SetPosition(GetPosition());
		dynamic_cast<PokemonSummaryMenu*>(ChildUIActor_)->InitPokemonSummaryMenu(CurrentOrder_);
		break;
	default:
		break;
	}
}

void PokemonMenu::OpeningChildUIUpdate()
{
	if (ChildUIActor_->IsUpdate() == false)
	{
		//Ż���Լ�
		FadeActor_->FadeOut();
		ChildUIActor_->Death();
		ChildUIActor_ = nullptr;
		CurChildUIType_ = ChildUIType::None;
		ChangeState(PrevMenuType_);
		return;



	}
}

void PokemonMenu::UsingPotionStart()
{
	DialogFont_->Off();
	DialogRenderer_->Off();
}

void PokemonMenu::UsingPotionUpdate()
{
	UsingPotionTime_ += GameEngineTime::GetDeltaTime();
	if (UsingPotionTime_ >= 0.03f)
	{
		UsingPotionTime_ = 0.0f;
		if (PokemonList_[CurrentOrder_]->GetHp() < PokemonList_[CurrentOrder_]->GetMaxHp() && PotionValue_ <20)
		{
			ChangeHp(CurrentOrder_, 1);
			PotionValue_++;
		}
	}
}


void PokemonMenu::InitRenderer()
{
	//��׶���
	BackgroundRenderer_ = CreateRenderer(static_cast<int>(GetOrder()), RenderPivot::LeftTop);
	BackgroundRenderer_->SetImage("PoketmonMenu_22.bmp");

	//���ϸ� �ڽ�
	BoxRenderer_[0] = CreateRenderer(static_cast<int>(GetOrder()), RenderPivot::LeftTop, { 8,80 });

	for (int i = 1; i < 6; i++)
	{
		BoxRenderer_[i] = CreateRenderer(static_cast<int>(GetOrder()), RenderPivot::LeftTop, { 352,static_cast<float>(-56 + 96 * i) });
	}

	BoxRenderer_[0]->SetImage("PoketmonMenu_14.bmp"); //Ŀ�ٶ� �ڽ�
	for (int i = 1; i < 6; i++)
	{
		BoxRenderer_[i]->SetImage("PoketmonMenu_12.bmp"); //���� �ڽ�
	}

	for (int i = 0; i < 6; i++)
	{
		BoxRenderer_[i]->SetTransColor(RGB(255, 0, 255));
	}

	for (int i = 0; i < 6; i++)
	{
		BoxRenderer_[i]->Off();
	}

	//��ȭ�� �ڽ�
	DialogRenderer_ = CreateRenderer(static_cast<int>(GetOrder()), RenderPivot::LeftTop, { 8,520 });
	DialogRenderer_->SetImage("PoketmonMenu_16.bmp");
	DialogRenderer_->SetTransColor(RGB(255, 0, 255));

	//������ ��ȭ�� �ڽ�
	QuestionDialogRenderer_ = CreateRenderer(static_cast<int>(GetOrder()), RenderPivot::LeftTop, { 8,520 });
	QuestionDialogRenderer_->SetImage("DialogBox_PokemonMenu_Question.bmp");
	QuestionDialogRenderer_->SetTransColor(RGB(255, 0, 255));
	QuestionDialogRenderer_->Off();

	//���� ���̰��� ������ �ڽ�
	UsingPotionBoxRenderer_ = CreateRenderer("PokemonMenu_UsingPotion.bmp", GetOrder(), RenderPivot::LeftTop, { 4,450 });
	UsingPotionBoxRenderer_->SetTransColor(RGB(255, 0, 255));

	//������ ��ȭ�� �ڽ�
	SelectDialogRenderer_ = CreateRenderer(static_cast<int>(GetOrder()), RenderPivot::LeftTop, { 580,326 });
	SelectDialogRenderer_->SetImage("DialogBox_PokemonMenu_Select.bmp");
	SelectDialogRenderer_->SetTransColor(RGB(255, 0, 255));
	SelectDialogRenderer_->Off();

	//ĵ�� ��ư
	CancelRenderer_ = CreateRenderer(static_cast<int>(GetOrder()), RenderPivot::LeftTop, { 732,530 });
	CancelRenderer_->SetImage("PoketmonMenu_10.bmp");
	CancelRenderer_->SetTransColor(RGB(255, 0, 255));

	//���ϸ� ������
	PokemonRenderer_[0] = CreateRenderer(static_cast<int>(GetOrder()+2), RenderPivot::BOT, { 65,200 });
	PokemonRenderer_[0]->CreateAnimation("Nullimage_Ani.bmp", "Null", 0, 1,0.1f,false);
	PokemonRenderer_[0]->ChangeAnimation("Null");
	PokemonRenderer_[0]->SetTransColor(RGB(255, 0, 255));
	PokemonRenderer_[0]->Off();

	for (int i = 1; i < 6; i++)
	{
		PokemonRenderer_[i] = CreateRenderer(static_cast<int>(GetOrder()+2), RenderPivot::BOT, { 406,static_cast<float>(40 + 96 * i) });
		PokemonRenderer_[i]->CreateAnimation("Nullimage_Ani.bmp", "Null", 0, 1, 0.1f, false);
		PokemonRenderer_[i]->ChangeAnimation("Null");
		PokemonRenderer_[i]->SetTransColor(RGB(255, 0, 255));
		PokemonRenderer_[i]->Off();
	}

	//HP�ڽ�
	HpRenderer_[0] = CreateRenderer(static_cast<int>(GetOrder()), RenderPivot::LeftTop, {128,236});
	HpRenderer_[0]->SetImage("PoketmonMenu_Hp1.bmp");
	HpRenderer_[0]->SetTransColor(RGB(255, 0, 255));

	for (int i = 1; i < 6; i++)
	{
		HpRenderer_[i] = CreateRenderer(static_cast<int>(GetOrder()), RenderPivot::LeftTop, { 736,static_cast<float>( -24+ 96 * i) });
		HpRenderer_[i]->SetImage("PoketmonMenu_Hp1.bmp");
		HpRenderer_[i]->SetTransColor(RGB(255, 0, 255));
		HpRenderer_[i]->Off();
	}

	//����
	GenderRenderer_	[0] = CreateRenderer(static_cast<int>(GetOrder()), RenderPivot::LeftTop, { 268,186 });
	GenderRenderer_[0]->SetImage("PoketmonMenu_20.bmp");
	GenderRenderer_[0]->SetTransColor(RGB(255, 0, 255));

	for (int i = 1; i < 6; i++)
	{
		GenderRenderer_[i] = CreateRenderer(static_cast<int>(GetOrder()), RenderPivot::LeftTop, { 636,static_cast<float>(+ 96 * i) });
		GenderRenderer_[i]->SetImage("PoketmonMenu_20.bmp");
		GenderRenderer_[i]->SetTransColor(RGB(255, 0, 255));
		GenderRenderer_[i]->Off();
	}

	//MenuArrow
	MenuArrowRenderer_ = CreateRenderer(static_cast<int>(GetOrder()), RenderPivot::LeftTop, { 610,370 });
	MenuArrowRenderer_->SetImage("Bag_CurrentArrow.bmp");
	MenuArrowRenderer_->SetTransColor(RGB(255, 0, 255));
	MenuArrowRenderer_->Off();

	//���ϸ� �̸��� ������ �ִϸ��̼��� �߰��� �ش�
	for (int i = 0; i < PokemonList_.size(); i++)
	{
		PokemonRenderer_[i]->CreateAnimation(PokemonList_[i]->GetMyIcon(), PokemonList_[i]->GetNameCopy(), 0, 1, 0.3f, true);
		PokemonRenderer_[i]->ChangeAnimation(PokemonList_[i]->GetNameCopy());
	}

	//���ϸ��� ������ �������� ������Ʈ ���ش�
	for (int i = 0; i < PokemonList_.size(); i++)
	{
		if (PokemonList_[i]->GetGender() == true) //����
		{
			GenderRenderer_[i]->SetImage("PoketmonMenu_20.bmp");
		}
		else
		{
			GenderRenderer_[i]->SetImage("PoketmonMenu_21.bmp");
		}

	}

}

void PokemonMenu::GetPlayerPokemon()
{

	//���⼭ �÷��̾� ����Ʈ�� �����ϰ� ��ĳ�� ������ŭ PokemonNumber�� �÷��ش�.
	{
		std::vector<Pokemon*>& Group = PlayerRed::MainRed_->GetPokemonList();
		for (Pokemon* i : Group)
		{
			PokemonList_.push_back(i->GetInfo());
			PokemonNumber_++;
		}
	}
}

void PokemonMenu::OnUI()
{
	for (int i = 0; i < PokemonNumber_; i++)
	{
		BoxRenderer_[i]->On();
		PokemonRenderer_[i]->On();
		HpRenderer_[i]->On();
		GenderRenderer_[i]->On();
	}

}

void PokemonMenu::ChangeHp(int _PokemonIndex, int _value)
{
	PokemonInfo* CurPokemonInfo = PokemonList_[_PokemonIndex];
	CurPokemonInfo->PlusHp(_value);
	//���� ��Ʈ ������Ʈ
	CurHpFonts_[_PokemonIndex]->ClearCurrentFonts();

	GameEngineContentFont* NewFonts = GetLevel()->CreateActor<GameEngineContentFont>(static_cast<int>(GetOrder()));
	if (_PokemonIndex == 0)
	{
		NewFonts->SetPosition({ 150,248 });
	}
	else
	{
		NewFonts->SetPosition({ 760,static_cast<float>(-16 + 96 * _PokemonIndex) });
	}
	NewFonts->ShowString(std::to_string(PokemonList_[_PokemonIndex]->GetHp()), true);

	CurHpFonts_[_PokemonIndex] = NewFonts;
}

void PokemonMenu::InitFont()
{
	//ù��° ���ϸ� ��Ʈ
	{
		//�̸�
		{
			GameEngineContentFont* NewFonts = GetLevel()->CreateActor<GameEngineContentFont>(GetOrder());
			NewFonts->SetPosition(GetPosition());
			NewFonts->ShowString(PokemonList_[0]->GetNameCopy(), true);
			PokemonNameFonts_.push_back(NewFonts);
		}

		//����
		{
			GameEngineContentFont* NewFonts = GetLevel()->CreateActor<GameEngineContentFont>(static_cast<int>(GetOrder()));
			NewFonts->SetPosition(GetPosition() +float4( 190,175 ));
			NewFonts->ShowString(std::to_string(PokemonList_[0]->GetMyLevel()), true);
			PokemonLevelFonts_.push_back(NewFonts);
		}

		//���� ü��
		{
			GameEngineContentFont* NewFonts = GetLevel()->CreateActor<GameEngineContentFont>(static_cast<int>(GetOrder()));
			NewFonts->SetPosition(GetPosition() +float4( 150,248 ));
			NewFonts->ShowString(std::to_string(PokemonList_[0]->GetHp()), true);
			CurHpFonts_.push_back(NewFonts);
		}

		//�ִ� ü��
		{
			GameEngineContentFont* NewFonts = GetLevel()->CreateActor<GameEngineContentFont>(static_cast<int>(GetOrder()));
			NewFonts->SetPosition(GetPosition() +float4( 260,248 ));
			NewFonts->ShowString(std::to_string(PokemonList_[0]->GetMaxHp()), true);
			MaxHpFonts_.push_back(NewFonts);
		}

	}
	
	//2° ~ �׵� ���ϸ�
	for (int i = 1; i < PokemonList_.size(); i++)
	{
		//PokemonRenderer_[i]->ChangeAnimation(PokemonList_[i]->GetNameCopy());
		//�̸�
		{
			GameEngineContentFont* NewFonts = GetLevel()->CreateActor<GameEngineContentFont>(static_cast<int>(GetOrder()));
			NewFonts->SetPosition(GetPosition() +float4( 416,static_cast<float>(-60 + 96 * i )));
			NewFonts->ShowString(PokemonList_[i]->GetNameCopy(), true);
			PokemonNameFonts_.push_back(NewFonts);
		}

		//����
		{
			GameEngineContentFont* NewFonts = GetLevel()->CreateActor<GameEngineContentFont>(static_cast<int>(GetOrder()));
			NewFonts->SetPosition(GetPosition() +float4(  540, static_cast<float>(-16 + 96 * i) ));
			NewFonts->ShowString(std::to_string(PokemonList_[i]->GetMyLevel()), true);
			PokemonLevelFonts_.push_back(NewFonts);
		}

		//���� ü��
		{
			GameEngineContentFont* NewFonts = GetLevel()->CreateActor<GameEngineContentFont>(static_cast<int>(GetOrder()));
			NewFonts->SetPosition(GetPosition() +float4( 760,static_cast<float>(-16 + 96 * i)));
			NewFonts->ShowString(std::to_string(PokemonList_[i]->GetHp()), true);
			CurHpFonts_.push_back(NewFonts);
		}

		//�ִ� ü��
		{
			GameEngineContentFont* NewFonts = GetLevel()->CreateActor<GameEngineContentFont>(static_cast<int>(GetOrder()));
			NewFonts->SetPosition(GetPosition() +float4( 870,static_cast<float>(-16 + 96 * i) ));
			NewFonts->ShowString(std::to_string(PokemonList_[i]->GetMaxHp()), true);
			MaxHpFonts_.push_back(NewFonts);
		}

	}
	//���ϸ��� ����ּ��� ��Ʈ
	{
		DialogFont_ = GetLevel()->CreateActor<GameEngineContentFont>(static_cast<int>(GetOrder()));
		DialogFont_->SetPosition(GetPosition() +float4( 16,540 ));
		DialogFont_->ShowString("Choose a pokemon", true);
	}
	//swtich ����
	{
		SwitchFont_ = GetLevel()->CreateActor<GameEngineContentFont>(static_cast<int>(GetOrder()));
		SwitchFont_->SetPosition(GetPosition() +float4( 16,540));
		SwitchFont_->ShowString("Move to Where?", true);
		SwitchFont_->Off();
	}

}


void PokemonMenu::MoveAllRenderer(int _index, float _Speed)
{
	if (_index == 0)
	{
		BoxRenderer_[0]->SetPivot(float4(BoxRenderer_[0]->GetPivot().x - GameEngineTime::GetDeltaTime() * _Speed, BoxRenderer_[0]->GetPivot().y));
		CurHpFonts_[0]->SetMove(float4::LEFT * GameEngineTime::GetDeltaTime() * _Speed);
		MaxHpFonts_[0]->SetMove(float4::LEFT * GameEngineTime::GetDeltaTime() * _Speed);
		PokemonRenderer_[0]->SetPivot(float4(PokemonRenderer_[0]->GetPivot().x - GameEngineTime::GetDeltaTime() * _Speed, PokemonRenderer_[0]->GetPivot().y));
		PokemonLevelFonts_[0]->SetMove(float4::LEFT * GameEngineTime::GetDeltaTime() * _Speed);
		HpRenderer_[0]->SetPivot(float4(HpRenderer_[0]->GetPivot().x - GameEngineTime::GetDeltaTime() * _Speed, HpRenderer_[0]->GetPivot().y));
		PokemonNameFonts_[0] -> SetMove(float4::LEFT * GameEngineTime::GetDeltaTime() * _Speed);
		GenderRenderer_[0]->SetPivot(float4(GenderRenderer_[0]->GetPivot().x - GameEngineTime::GetDeltaTime() * _Speed, GenderRenderer_[0]->GetPivot().y));
	}
	else
	{
		BoxRenderer_[_index]->SetPivot(float4(BoxRenderer_[_index]->GetPivot().x + GameEngineTime::GetDeltaTime() * _Speed, BoxRenderer_[_index]->GetPivot().y));
		CurHpFonts_[_index]->SetMove(float4::RIGHT * GameEngineTime::GetDeltaTime() * _Speed);
		MaxHpFonts_[_index]->SetMove(float4::RIGHT * GameEngineTime::GetDeltaTime() * _Speed);
		PokemonRenderer_[_index]->SetPivot(float4(PokemonRenderer_[_index]->GetPivot().x + GameEngineTime::GetDeltaTime() * _Speed, PokemonRenderer_[_index]->GetPivot().y));
		PokemonLevelFonts_[_index]->SetMove(float4::RIGHT * GameEngineTime::GetDeltaTime() * _Speed);
		HpRenderer_[_index]->SetPivot(float4(HpRenderer_[_index]->GetPivot().x + GameEngineTime::GetDeltaTime() * _Speed, HpRenderer_[_index]->GetPivot().y));
		PokemonNameFonts_[_index]->SetMove(float4::RIGHT * GameEngineTime::GetDeltaTime() * _Speed);
		GenderRenderer_[_index]->SetPivot(float4(GenderRenderer_[_index]->GetPivot().x + GameEngineTime::GetDeltaTime() * _Speed, GenderRenderer_[_index]->GetPivot().y));
	}

}

void PokemonMenu::IconJump()
{
	CurTickTime_ += GameEngineTime::GetDeltaTime();
	//����
	if (CurTickTime_ < 0.15f)
	{
		if (CanJump_[0] == true)
		{
			PokemonRenderer_[0]->SetPivot({ 65,170 });
		}
		for (size_t i = 1; i < PokemonNumber_; i++)
		{
			if (CanJump_[i] == true)
			{
				PokemonRenderer_[i]->SetPivot({ 406, static_cast<float>(10 + 96 * i) });
			}
		}
	}
	else if (CurTickTime_ >= 0.15f && CurTickTime_ < 0.3f)	//����
	{
		if (CanJump_[0] == true)
		{
			PokemonRenderer_[0]->SetPivot({ 65,200 });
		}
		for (size_t i = 1; i < PokemonNumber_; i++)
		{
			PokemonRenderer_[i]->SetPivot({ 406, static_cast<float>(40 + 96 * i) });
		}
	}
	else //�ٽ� �����ϰ� �ð� �ʱ�ȭ
	{
		CurTickTime_ = 0;
	}


}


void PokemonMenu::IconJumpOn(int _PokemonOrder)
{
	CanJump_[_PokemonOrder] = true;
}

void PokemonMenu::IconJumpOff(int _PokemonOrder)
{
	CanJump_[_PokemonOrder] = false;
}


void PokemonMenu::InitPokemonMenu(Item* _Item)
{
	//FadeActor ����
	FadeActor_ = GetLevel()->CreateActor<FadeActor>();
	FadeActor_->SetPosition(GetPosition()+GameEngineWindow::GetScale().Half());

	//��ó�� �̺�Ʈ
	if (_Item != nullptr)
	{
		IsItemMode_ = true;
	}

	GetPlayerPokemon();
	InitRenderer();
	InitFont();
	OnUI();
	IsOn_ = true;
}

void PokemonMenu::DestroyPokemonMenu()
{
	for (GameEngineContentFont* i : PokemonNameFonts_)
	{
		i->ClearCurrentFonts();
	}

	for (GameEngineContentFont* i : CurHpFonts_)
	{
		i->ClearCurrentFonts();
	}

	for (GameEngineContentFont* i : PokemonLevelFonts_)
	{
		i->ClearCurrentFonts();
	}

	for (GameEngineContentFont* i : MaxHpFonts_)
	{
		i->ClearCurrentFonts();
	}

	DialogFont_->ClearCurrentFonts();
	SwitchFont_->ClearCurrentFonts();
	if (QuestionFont_ != nullptr)
	{
		QuestionFont_->ClearCurrentFonts();
	}

	//���̵�ƿ�
	FadeActor_->FadeOut();
	Off();
}




