#include "PokemonMenu.h"
#include "UIEnum.h"
#include <GameEngine/GameEngineRenderer.h>
#include <GameEngineBase/GameEngineInput.h>
#include "Pokemon.h"
#include <GameEngineContentsCore/GameEngineContentFont.h>
#include "PokemonInfoManager.h"
#include <GameEngine/GameEngineImageManager.h>
#include <GameEngine/GameEngine.h>


PokemonMenu::PokemonMenu():
	BackgroundRenderer_(nullptr),
	PokemonNumber_(0),
	DialogRenderer_(nullptr),
	CancelRenderer_(nullptr),
	CurrentOrder_(0),
	RememberOrder_(0),
	CurState_(PokemonMenuType::SelectPokemon),
	CurTickTime_(0),
	IsJump_(0)
{
}

PokemonMenu::~PokemonMenu()
{
}

void PokemonMenu::Start()
{
	//NullImage_Ani �����̽�
	GameEngineImage* NullImage = GameEngineImageManager::GetInst()->Find("NullImage_Ani.bmp");
	NullImage->Cut({ 32,32 });

	InitRenderer();
	OnUI();

	////��Ʈ ��� �׽�Ʈ
	{
		GameEngineContentFont* Fonts = GetLevel()->CreateActor<GameEngineContentFont>(static_cast<int>(UIRenderType::Font));
		Fonts->SetPosition({ 16,540 });
		Fonts->ShowString("Please choose a pokemon",true);
		AllFonts_.push_back(Fonts);
	}


}

void PokemonMenu::Update()
{
	UpdateState();	


}

void PokemonMenu::Render()
{
	//���ϸ� �̹��� ���� ������
	if (CurrentOrder_ == 0) //���õ� ���
	{
		BoxRenderer_[0]->SetPivot({ 8,72 });
		BoxRenderer_[0]->SetImage("PoketmonMenu_15.bmp"); //Ŀ�ٶ� �ڽ�
		IconJump(PokemonRenderer_[0]);
	}
	
	else
	{
		BoxRenderer_[0]->SetPivot({ 8,80 });
		BoxRenderer_[0]->SetImage("PoketmonMenu_14.bmp"); //Ŀ�ٶ� �ڽ�
		PokemonRenderer_[0]->SetPivot({ 65,200 });
	}

	for (int i = 1; i < 6; i++)
	{
		if (CurrentOrder_ == i) //���õ� ���
		{
			BoxRenderer_[i]->SetPivot({ 352,static_cast<float>(-60 + 96 * i) });
			BoxRenderer_[i]->SetImage("PoketmonMenu_13.bmp"); //���� �ڽ�
			IconJump(PokemonRenderer_[i]);
		}
		else //���� �ȵ� ���
		{
			BoxRenderer_[i]->SetPivot({ 352,static_cast<float>(-56 + 96 * i) });
			BoxRenderer_[i]->SetImage("PoketmonMenu_12.bmp"); //���� �ڽ�
			PokemonRenderer_[i]->SetPivot({ 406, static_cast<float>(40 + 96 * i) });
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

	//LPPOINT mousePos; // ���콺 ��ǥ�� ������ ���� ����. POINT �ڷ����� ����������.
	//mousePos = new POINT;
	//GetCursorPos(mousePos); // ����ȭ�鿡���� ���콺 ��ǥ�� ������
	//ScreenToClient(nullptr, mousePos);
	//int x = mousePos->x;
	//TextOut(GameEngine::BackBufferDC(), 100, 100, std::to_string(x).c_str(), std::to_string(x).length());
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
	default:
		break;
	}
}

void PokemonMenu::SelectPokemonStart()
{
	DialogRenderer_->On();
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
			ResetJump();
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
			ResetJump();
			CurrentOrder_--;
		}
	}

	if (GameEngineInput::GetInst()->IsDown("Left") == true)
	{
		if (CurrentOrder_ != PokemonNumber_ && CurrentOrder_ != 0)
		{
			ResetJump();
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
				ResetJump();
				RememberOrder_ = 1;
			}
			ResetJump();
			CurrentOrder_ = RememberOrder_;
		}
	}
}

void PokemonMenu::SelectActionStart()
{
}

void PokemonMenu::SelectActionUpdate()
{
}



void PokemonMenu::InitRenderer()
{
	//��׶���
	BackgroundRenderer_ = CreateRenderer(static_cast<int>(UIRenderType::Background), RenderPivot::LeftTop);
	BackgroundRenderer_->SetImage("PoketmonMenu_22.bmp");

	//���ϸ� �ڽ�
	BoxRenderer_[0] = CreateRenderer(static_cast<int>(UIRenderType::Box), RenderPivot::LeftTop, { 8,80 });

	for (int i = 1; i < 6; i++)
	{
		BoxRenderer_[i] = CreateRenderer(static_cast<int>(UIRenderType::Box), RenderPivot::LeftTop, { 352,static_cast<float>(-56 + 96 * i) });
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
	DialogRenderer_ = CreateRenderer(static_cast<int>(UIRenderType::Box), RenderPivot::LeftTop, { 8,520 });
	DialogRenderer_->SetImage("PoketmonMenu_16.bmp");
	DialogRenderer_->SetTransColor(RGB(255, 0, 255));

	//ĵ�� ��ư
	CancelRenderer_ = CreateRenderer(static_cast<int>(UIRenderType::Box), RenderPivot::LeftTop, { 732,530 });
	CancelRenderer_->SetImage("PoketmonMenu_10.bmp");
	CancelRenderer_->SetTransColor(RGB(255, 0, 255));

	//���ϸ� ������
	PokemonRenderer_[0] = CreateRenderer(static_cast<int>(UIRenderType::Object), RenderPivot::BOT, { 65,200 });
	PokemonRenderer_[0]->CreateAnimation("Nullimage_Ani.bmp", "Null", 0, 1,0.1f,false);
	PokemonRenderer_[0]->ChangeAnimation("Null");
	PokemonRenderer_[0]->SetTransColor(RGB(255, 0, 255));
	PokemonRenderer_[0]->Off();

	for (int i = 1; i < 6; i++)
	{
		PokemonRenderer_[i] = CreateRenderer(static_cast<int>(UIRenderType::Object), RenderPivot::BOT, { 406,static_cast<float>(40 + 96 * i) });
		PokemonRenderer_[i]->CreateAnimation("Nullimage_Ani.bmp", "Null", 0, 1, 0.1f, false);
		PokemonRenderer_[i]->ChangeAnimation("Null");
		PokemonRenderer_[i]->SetTransColor(RGB(255, 0, 255));
		PokemonRenderer_[i]->Off();
	}
}

void PokemonMenu::GetPlayerPokemon()
{

	//���⼭ �÷��̾� ����Ʈ�� �����ϰ� ��ĳ�� ������ŭ PokemonNumber�� �÷��ش�.
	{
		PokemonInfo* Charmander = PokemonInfoManager::GetInst().FindPokemonInfo("Charmander");
		PokemonInfo* Squirtle = PokemonInfoManager::GetInst().FindPokemonInfo("Squirtle"); //Bulbasaur
		PokemonInfo* Bulbasaur = PokemonInfoManager::GetInst().FindPokemonInfo("Bulbasaur");

		PokemonList_.push_back(Charmander);
		PokemonList_.push_back(Squirtle);
		PokemonList_.push_back(Bulbasaur);
		PokemonNumber_ = 3;

		//���ϸ� �̸��� ������ �ִϸ��̼��� �߰��� �ش�
		for (int i = 0; i < PokemonList_.size(); i++)
		{
			PokemonRenderer_[i]->CreateAnimation(PokemonList_[i]->GetMyIcon(), PokemonList_[i]->GetNameCopy(), 0, 1, 0.3f, true);
			PokemonRenderer_[i]->ChangeAnimation(PokemonList_[i]->GetNameCopy());


		}
	}
}

void PokemonMenu::OnUI()
{
	GetPlayerPokemon();
	InitFont();
	for (int i = 0; i < PokemonNumber_; i++)
	{
		BoxRenderer_[i]->On();
		PokemonRenderer_[i]->On();
	}

}

void PokemonMenu::InitFont()
{
	//ù��° ���ϸ� ��Ʈ
	{
		//�̸�
		{
			GameEngineContentFont* NewFonts = GetLevel()->CreateActor<GameEngineContentFont>(static_cast<int>(UIRenderType::Font));
			NewFonts->SetPosition({ 75,126 });
			NewFonts->ShowString(PokemonList_[0]->GetNameCopy(), true);
			AllFonts_.push_back(NewFonts);
		}

		//����
		{
			GameEngineContentFont* NewFonts = GetLevel()->CreateActor<GameEngineContentFont>(static_cast<int>(UIRenderType::Font));
			NewFonts->SetPosition({ 190,175 });
			NewFonts->ShowString(std::to_string(PokemonList_[0]->GetMyLevel()), true);
			AllFonts_.push_back(NewFonts);
		}

		//���� ü��
		{
			GameEngineContentFont* NewFonts = GetLevel()->CreateActor<GameEngineContentFont>(static_cast<int>(UIRenderType::Font));
			NewFonts->SetPosition({ 150,248 });
			NewFonts->ShowString(std::to_string(PokemonList_[0]->GetHp()), true);
			AllFonts_.push_back(NewFonts);
		}

		//�ִ� ü��
		{
			GameEngineContentFont* NewFonts = GetLevel()->CreateActor<GameEngineContentFont>(static_cast<int>(UIRenderType::Font));
			NewFonts->SetPosition({ 260,248 });
			NewFonts->ShowString(std::to_string(PokemonList_[0]->GetMaxHp()), true);
			AllFonts_.push_back(NewFonts);
		}

	}
	
	//2° ~ �׵� ���ϸ�
	for (int i = 1; i < PokemonList_.size(); i++)
	{
		//PokemonRenderer_[i]->ChangeAnimation(PokemonList_[i]->GetNameCopy());
		//�̸�
		{
			GameEngineContentFont* NewFonts = GetLevel()->CreateActor<GameEngineContentFont>(static_cast<int>(UIRenderType::Font));
			NewFonts->SetPosition({ 416,static_cast<float>(-60 + 96 * i )});
			NewFonts->ShowString(PokemonList_[i]->GetNameCopy(), true);
			AllFonts_.push_back(NewFonts);
		}

		//����
		{
			GameEngineContentFont* NewFonts = GetLevel()->CreateActor<GameEngineContentFont>(static_cast<int>(UIRenderType::Font));
			NewFonts->SetPosition({ 540,static_cast<float>(-16 + 96 * i) });
			NewFonts->ShowString(std::to_string(PokemonList_[i]->GetMyLevel()), true);
			AllFonts_.push_back(NewFonts);
		}

	}


}

void PokemonMenu::IconJump(GameEngineRenderer* _Render)
{
	CurTickTime_ += GameEngineTime::GetDeltaTime();
	if (CurTickTime_ > 0.15f)
	{
		CurTickTime_ = 0;
		if (IsJump_ == true)
		{
			_Render->SetPivot({ _Render->GetPivot().x,_Render->GetPivot().y + 30});
			IsJump_ = false;	
		}
		else
		{
			_Render->SetPivot({ _Render->GetPivot().x,_Render->GetPivot().y - 30 });
			IsJump_ = true;
		}
	}
}

void PokemonMenu::ResetJump()
{
	IsJump_ = false;
	CurTickTime_ = 0;
}




