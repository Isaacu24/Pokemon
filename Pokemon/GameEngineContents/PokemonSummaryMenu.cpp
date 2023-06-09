#include "PokemonSummaryMenu.h"
#include <GameEngine/GameEngineRenderer.h>
#include <GameEngineBase/GameEngineInput.h>
#include <GameEngineContentsCore/GameEngineContentFont.h>

#include "PokemonSkillInfo.h"
#include "PokemonSkill.h"
#include "PlayerRed.h"
#include "Pokemon.h"
#include "PokemonInfo.h"
#include "WorldMapSoundManager.h"
#include "ContentEnum.h"

PokemonSummaryMenu::PokemonSummaryMenu() :
	TopMenuRenderer_(nullptr),
	CurState_(PokemonSummaryMenuType::PokemonInfo),
	PokemonFrameRenderer_(nullptr),
	PokemonInfoRenderer_(nullptr),
	IsOn_(false),
	CurrentOrder_(0),
	AccGravity_(0.0f),
	IsJumpStart_(false),
	CurYPos_(0),
	JumpTime_(0.0f),
	Owner_(nullptr),
	IDNumber_(nullptr),
	PokemonSkillGreenBoxRenderer_(nullptr),
	PokemonSkillSelect_PokemonFrameRenderer_(nullptr),
	PokemonSkillSelect_LeftGreenBox(nullptr),
	SkillSelect_CurrentOrder_(0)

{
	PokemonInfoList_.reserve(6);
	PokemonFrontRenderer_.reserve(6);
	LevelFonts_.reserve(6);
	AllFonts_.reserve(60);
	NumberFonts_.reserve(6);
	FrameNameFonts_.reserve(6);
	GenderRenderer_.reserve(6);

	CurHpFonts_.reserve(6);
	MaxHpFonts_.reserve(6);
	AttFonts_.reserve(6);
	DefFonts_.reserve(6);
	SpAttFonts_.reserve(6);
	SpDefFonts_.reserve(6);
	SpeedFonts_.reserve(6);
	CurExpFonts_.reserve(6);
	NextExpFonts_.reserve(6);
	AbilityFonts_.reserve(6);
	AbilityExplanationFonts_.reserve(6);
	PokemonSkillSelect_MiniPokemonRenderer_.reserve(6);
	TypeString_.reserve(6);
	PokemonSkillSelect_TypeRenderer_.reserve(6);
	


	for (size_t i = 0; i < 6; i++)
	{
		SkillNameFonts_[i].reserve(4);
		CurSkillPPFonts_[i].reserve(4);
		MaxSkillPPFonts_[i].reserve(4);
		PokemonSkillTypeRenderer_[i].reserve(4);
		SkillValueFonts_[i].reserve(4);
		SkillAccFonts_[i].reserve(4);
		SkillExplanationFonts_[i].reserve(4);
		PokemonSkillSelect_SelectLineRenderer_[i].reserve(4);
	}

	

}

PokemonSummaryMenu::~PokemonSummaryMenu()
{
}

void PokemonSummaryMenu::InitPokemonSummaryMenu(int _PokemonOrder)
{
	CurrentOrder_ = _PokemonOrder;
	GetPlayerPokemon();
	InitRenderer_();
	InitFonts_();

	//필요한 것만 On
	PokemonFrontRenderer_[CurrentOrder_]->On();
	PokemonTypeRenderer_[CurrentOrder_]->On();
	PokemonJumpStart();
	IsOn_ = true;
}

void PokemonSummaryMenu::GetPlayerPokemon()
{
	for (Pokemon* i : PlayerRed::MainRed_->GetPokemonList())
	{
		PokemonInfoList_.push_back(i->GetInfo());
	}
}

void PokemonSummaryMenu::Start()
{

}

void PokemonSummaryMenu::Update()
{
	if (IsOn_ == false)
	{
		return;
	}
	PokemonJump();
	UpdateState();
}

void PokemonSummaryMenu::Render()
{
	switch (CurState_)
	{
	case PokemonSummaryMenu::PokemonSummaryMenuType::PokemonInfo:
		TopMenuRenderer_->SetImage("PokemonInfo.bmp");
		PokemonInfoRenderer_->SetImage("PokemonInfo_Window.bmp");
		PokemonSkillGreenBoxRenderer_->Off();
		IDNumber_->On();
		Owner_->On();
		Item_->On();
		for (size_t i = 0; i < PokemonInfoList_.size(); i++)
		{
			if (i != CurrentOrder_)
			{
				PokemonTypeRenderer_[i]->Off();
				PokemonFrontRenderer_[i]->Off();
				LevelFonts_[i]->Off();
				NameFonts_[i]->Off();
				NumberFonts_[i]->Off();
				NatureFonts_[i]->Off();
				FrameNameFonts_[i]->Off();
				GenderRenderer_[i]->Off();
			}
			else
			{
				PokemonTypeRenderer_[i]->On();
				PokemonFrontRenderer_[i]->On();
				LevelFonts_[i]->On();
				NameFonts_[i]->On();
				NumberFonts_[i]->On();
				NatureFonts_[i]->On();
				FrameNameFonts_[i]->On();
				GenderRenderer_[i]->On();
			}

			//다른 State에서 쓰이는 폰트 전부 Off
			{
				//======= Pokemon Ability =========
				CurHpFonts_[i]->Off();
				MaxHpFonts_[i]->Off();
				AttFonts_[i]->Off();
				DefFonts_[i]->Off();
				SpAttFonts_[i]->Off();
				SpDefFonts_[i]->Off();
				SpeedFonts_[i]->Off();
				CurExpFonts_[i]->Off();
				NextExpFonts_[i]->Off();
				AbilityFonts_[i]->Off();
				AbilityExplanationFonts_[i]->Off();

			}
		}

		//======== Pokemon Skill ====================================
		{
			//스킬 타입 렌더러
			for (size_t i = 0; i < PokemonInfoList_.size(); i++)
			{
				for (size_t j = 0; j < PokemonSkillTypeRenderer_[i].size(); j++)
				{
					PokemonSkillTypeRenderer_[i][j]->Off();
				}
			}

			for (size_t i = 0; i < PokemonInfoList_.size(); i++)
			{
				for (size_t j = 0; j < SkillNameFonts_[i].size(); j++)
				{
					SkillNameFonts_[i][j]->Off();
				}
			}

			for (size_t i = 0; i < PokemonInfoList_.size(); i++)
			{
				for (size_t j = 0; j < CurSkillPPFonts_[i].size(); j++)
				{
					CurSkillPPFonts_[i][j]->Off();
				}
			}

			for (size_t i = 0; i < PokemonInfoList_.size(); i++)
			{
				for (size_t j = 0; j < MaxSkillPPFonts_[i].size(); j++)
				{
					MaxSkillPPFonts_[i][j]->Off();
				}
			}

		}
		break;

	case PokemonSummaryMenu::PokemonSummaryMenuType::PokemonAbility:
		TopMenuRenderer_->SetImage("PokemonAbility.bmp");
		PokemonInfoRenderer_->SetImage("PokemonAbility_Window.bmp");
		PokemonSkillGreenBoxRenderer_->Off();
		for (size_t i = 0; i < PokemonInfoList_.size(); i++)
		{
			if (i != CurrentOrder_)
			{
				PokemonFrontRenderer_[i]->Off();
				LevelFonts_[i]->Off();
				FrameNameFonts_[i]->Off();
				GenderRenderer_[i]->Off();
				CurHpFonts_[i]->Off();
				MaxHpFonts_[i]->Off();
				AttFonts_[i]->Off();
				DefFonts_[i]->Off();
				SpAttFonts_[i]->Off();
				SpDefFonts_[i]->Off();
				SpeedFonts_[i]->Off();
				CurExpFonts_[i]->Off();
				NextExpFonts_[i]->Off();
				AbilityExplanationFonts_[i]->Off();
				AbilityFonts_[i]->Off();

			}
			else
			{
				PokemonFrontRenderer_[i]->On();
				LevelFonts_[i]->On();
				FrameNameFonts_[i]->On();
				GenderRenderer_[i]->On();
				CurHpFonts_[i]->On();
				MaxHpFonts_[i]->On();
				AttFonts_[i]->On();
				DefFonts_[i]->On();
				SpAttFonts_[i]->On();
				SpDefFonts_[i]->On();
				SpeedFonts_[i]->On();
				CurExpFonts_[i]->On();
				AbilityFonts_[i]->On();
				AbilityExplanationFonts_[i]->On();
				NextExpFonts_[i]->On();
			}
		}

		//다른 State에서 쓰이는 폰트 끄기

		//===== Pokemon Info ==========
		Owner_->Off();
		Item_->Off();
		IDNumber_->Off();
		for (size_t i = 0; i < PokemonInfoList_.size(); i++)
		{
			NumberFonts_[i]->Off();
			NatureFonts_[i]->Off();
			PokemonTypeRenderer_[i]->Off();
			NameFonts_[i]->Off();
		}

		//======== Pokemon Skill ====================================
		{
			//스킬 타입 렌더러
			for (size_t i = 0; i < PokemonInfoList_.size(); i++)
			{
				for (size_t j = 0; j < PokemonSkillTypeRenderer_[i].size(); j++)
				{
					PokemonSkillTypeRenderer_[i][j]->Off();
				}
			}

			for (size_t i = 0; i < PokemonInfoList_.size(); i++)
			{
				for (size_t j = 0; j < SkillNameFonts_[i].size(); j++)
				{
					SkillNameFonts_[i][j]->Off();
				}
			}

			for (size_t i = 0; i < PokemonInfoList_.size(); i++)
			{
				for (size_t j = 0; j < CurSkillPPFonts_[i].size(); j++)
				{
					CurSkillPPFonts_[i][j]->Off();
				}
			}

			for (size_t i = 0; i < PokemonInfoList_.size(); i++)
			{
				for (size_t j = 0; j < MaxSkillPPFonts_[i].size(); j++)
				{
					MaxSkillPPFonts_[i][j]->Off();
				}
			}
		}
		break;


	case PokemonSummaryMenu::PokemonSummaryMenuType::PokemonSkill:
		TopMenuRenderer_->SetImage("PokemonSkill.bmp");
		PokemonInfoRenderer_->SetImage("PokemonSkill_Back.bmp");
		PokemonSkillGreenBoxRenderer_->On();
		PokemonFrameRenderer_->On();


		//처음 렌더러 끄는 부분
		{
			//스킬 타입 렌더러
			for (size_t i = 0; i < PokemonInfoList_.size(); i++)
			{
				for (size_t j = 0; j < PokemonSkillTypeRenderer_[i].size(); j++)
				{
					PokemonSkillTypeRenderer_[i][j]->Off();
				}
			}

			for (size_t i = 0; i < PokemonInfoList_.size(); i++)
			{
				for (size_t j = 0; j < SkillNameFonts_[i].size(); j++)
				{
					SkillNameFonts_[i][j]->Off();
				}
			}

			for (size_t i = 0; i < PokemonInfoList_.size(); i++)
			{
				for (size_t j = 0; j < CurSkillPPFonts_[i].size(); j++)
				{
					CurSkillPPFonts_[i][j]->Off();
				}
			}

			for (size_t i = 0; i < PokemonInfoList_.size(); i++)
			{
				for (size_t j = 0; j < MaxSkillPPFonts_[i].size(); j++)
				{
					MaxSkillPPFonts_[i][j]->Off();
				}
			}
		}

		//스킬 타입 렌더러
		for (size_t i = 0; i < PokemonSkillTypeRenderer_[CurrentOrder_].size(); i++)
		{
			PokemonSkillTypeRenderer_[CurrentOrder_][i]->On();
		}
		//스킬 이름 렌더러
		for (size_t j = 0; j < SkillNameFonts_[CurrentOrder_].size(); j++)
		{
			SkillNameFonts_[CurrentOrder_][j]->On();
		}
		// 현재 pp
		for (size_t i = 0; i < CurSkillPPFonts_[CurrentOrder_].size(); i++)
		{
			CurSkillPPFonts_[CurrentOrder_][i]->On();
		}
		//Max PP
		for (size_t i = 0; i < MaxSkillPPFonts_[CurrentOrder_].size(); i++)
		{
			MaxSkillPPFonts_[CurrentOrder_][i]->On();
		}

		for (size_t i = 0; i < PokemonInfoList_.size(); i++)
		{
			if (i != CurrentOrder_)
			{
				PokemonFrontRenderer_[i]->Off();
				LevelFonts_[i]->Off();
				FrameNameFonts_[i]->Off();
			}
			else
			{
				PokemonFrontRenderer_[i]->On();
				LevelFonts_[i]->On();
				FrameNameFonts_[i]->On();
			}
		}




		//다른 State에서 사용하는 폰트, 렌더러 제거
		PokemonSkillSelect_PokemonFrameRenderer_->Off();
		PokemonSkillSelect_LeftGreenBox->Off();
		for (size_t i = 0; i < PokemonInfoList_.size(); i++)
		{
			{
				//======= Pokemon Ability =========
				{
					CurHpFonts_[i]->Off();
					MaxHpFonts_[i]->Off();
					AttFonts_[i]->Off();
					DefFonts_[i]->Off();
					SpAttFonts_[i]->Off();
					SpDefFonts_[i]->Off();
					SpeedFonts_[i]->Off();
					CurExpFonts_[i]->Off();
					NextExpFonts_[i]->Off();
					AbilityFonts_[i]->Off();
					AbilityExplanationFonts_[i]->Off();
				}

				//=============== PokemonSkillSelect ================================
				{
					PokemonSkillSelect_TypeRenderer_[i]->Off();
					PokemonSkillSelect_MiniPokemonRenderer_[i]->Off();
				}
			}
			
		}

		
		
		for (size_t j = 0; j < PokemonInfoList_.size(); j++)
		{
			for (GameEngineRenderer* i : PokemonSkillSelect_SelectLineRenderer_[j])
			{
				i->Off();
			}
		}
		for (size_t j = 0; j < PokemonInfoList_.size(); j++)
		{
			for (GameEngineContentFont* i : SkillValueFonts_[j])
			{
				i->Off();
			}
		}
		
		for (size_t j = 0; j < PokemonInfoList_.size(); j++)
		{
			for (GameEngineContentFont* i : SkillAccFonts_[j])
			{
				i->Off();
			}
		}
		for (size_t j = 0; j < PokemonInfoList_.size(); j++)
		{
			for (GameEngineContentFont* i : SkillExplanationFonts_[j])
			{
				i->Off();
			}
		}
		
		break;


	case PokemonSummaryMenu::PokemonSummaryMenuType::PokemonSkillSelect:

		//렌더러 끄기
	{
		PokemonFrameRenderer_->Off();
		for (size_t i = 0; i < PokemonInfoList_.size(); i++)
		{
			LevelFonts_[i]->Off();
			PokemonFrontRenderer_[i]->Off();
			PokemonSkillSelect_MiniPokemonRenderer_[i]->Off();
			PokemonSkillSelect_TypeRenderer_[i]->Off();

		}
		
		for (size_t j = 0; j < PokemonInfoList_.size(); j++)
		{
			for (GameEngineRenderer* i : PokemonSkillSelect_SelectLineRenderer_[j]) //옌 스킬갯수가 다른 인덱스랑 다르니 별도로
			{
				i->Off();
			}
		}
		for (size_t j = 0; j < PokemonInfoList_.size(); j++)
		{
			for (GameEngineContentFont* i : SkillValueFonts_[j])
			{
				i->Off();
			}
		}

		for (size_t j = 0; j < PokemonInfoList_.size(); j++)
		{
			for (GameEngineContentFont* i : SkillAccFonts_[j])
			{
				i->Off();
			}
		}
		for (size_t j = 0; j < PokemonInfoList_.size(); j++)
		{
			for (GameEngineContentFont* i : SkillExplanationFonts_[j])
			{
				i->Off();
			}
		}
	}

	// 렌더러 켜기
	{
		PokemonSkillSelect_PokemonFrameRenderer_->On();
		PokemonSkillSelect_MiniPokemonRenderer_[CurrentOrder_]->On();
		PokemonSkillSelect_TypeRenderer_[CurrentOrder_]->On();
		PokemonSkillSelect_LeftGreenBox->On();
		PokemonSkillSelect_SelectLineRenderer_[CurrentOrder_][SkillSelect_CurrentOrder_]->On();
		SkillValueFonts_[CurrentOrder_][SkillSelect_CurrentOrder_]->On();
		SkillAccFonts_[CurrentOrder_][SkillSelect_CurrentOrder_]->On();
		SkillExplanationFonts_[CurrentOrder_][SkillSelect_CurrentOrder_]->On();

	}
		break;
	case PokemonSummaryMenu::PokemonSummaryMenuType::Max:
		break;
	default:
		break;
	}
	
}

void PokemonSummaryMenu::InitRenderer_()
{
	//기본 백그라운드 
	TopMenuRenderer_ = CreateRenderer("PokemonInfo.bmp", GetOrder(), RenderPivot::LeftTop);
	TopMenuRenderer_->SetTransColor(RGB(255, 0, 255));

	PokemonFrameRenderer_ = CreateRenderer("Pokemon_Frame.bmp", GetOrder(), RenderPivot::LeftTop,{0,64});
	PokemonFrameRenderer_->SetTransColor(RGB(255, 0, 255));

	PokemonInfoRenderer_ = CreateRenderer("PokemonInfo_Window.bmp", GetOrder()-1, RenderPivot::LeftTop, { 0,64 });
	PokemonInfoRenderer_->SetTransColor(RGB(255, 0, 255));

	PokemonSkillGreenBoxRenderer_ = CreateRenderer("PokemonSkill_Window.bmp", GetOrder(), RenderPivot::LeftTop, { 476,64 });
	PokemonSkillGreenBoxRenderer_->SetTransColor(RGB(255, 0, 255));
	PokemonSkillGreenBoxRenderer_->Off();

	PokemonSkillSelect_PokemonFrameRenderer_ = CreateRenderer("PokemonSkill_PokemonFrame.bmp", GetOrder()-1, RenderPivot::LeftTop, { 0,64 });
	PokemonSkillSelect_PokemonFrameRenderer_->SetTransColor(RGB(255, 0, 255));
	PokemonSkillSelect_PokemonFrameRenderer_->Off();

	PokemonSkillSelect_LeftGreenBox = CreateRenderer("PokemonSkill_Detail_Window.bmp", GetOrder() - 1, RenderPivot::LeftTop, { 0,196});
	PokemonSkillSelect_LeftGreenBox->SetTransColor(RGB(255, 0, 255));
	PokemonSkillSelect_LeftGreenBox->Off();

	

	//포켓몬 성별
	for (size_t i = 0; i < PokemonInfoList_.size(); i++)
	{
		GameEngineRenderer* NewRenderer = CreateRenderer( GetOrder(), RenderPivot::LeftTop, { 450,80 });
		if (PokemonInfoList_[i]->GetGender() == true)
		{
			NewRenderer->SetImage("PoketmonMenu_20.bmp");
		}
		else
		{
			NewRenderer->SetImage("PoketmonMenu_21.bmp");
		}
		NewRenderer->SetTransColor(RGB(255, 0, 255));
		NewRenderer->Off();
		GenderRenderer_.push_back(NewRenderer);
	}

	//포켓몬 앞면
	for (size_t i = 0; i < PokemonInfoList_.size(); i++)
	{
		GameEngineRenderer* NewRenderer = CreateRenderer(PokemonInfoList_[i]->GetMyBattleFront(), GetOrder(), RenderPivot::LeftTop,{120,120});
		NewRenderer->SetTransColor(RGB(255, 0, 255));
		NewRenderer->Off();
		PokemonFrontRenderer_.push_back(NewRenderer);
	}

	//포켓몬 아이콘
	for (size_t i = 0; i < PokemonInfoList_.size(); i++)
	{
		GameEngineRenderer* NewRenderer = CreateRenderer(PokemonInfoList_[i]->GetMyIcon(), GetOrder(), RenderPivot::LeftTop, { 40,100 });
		NewRenderer->SetIndex(0);
		NewRenderer->SetTransColor(RGB(255, 0, 255));
		NewRenderer->Off();
		PokemonSkillSelect_MiniPokemonRenderer_.push_back(NewRenderer);
	}


	//타입
	for (size_t i = 0; i < PokemonInfoList_.size(); i++)
	{
		std::string TypeString;
		PokemonType Type = PokemonInfoList_[i]->GetMyType();

		switch (Type)
		{
		case PokemonType::NORMAL:
			TypeString = "Normal";
			break;
		case PokemonType::FIRE:
			TypeString = "Fire";
			break;
		case PokemonType::WATER:
			TypeString = "Water";
			break;
		case PokemonType::GRASS:
			TypeString = "Grass";
			break;
		case PokemonType::ELECTRIC:
			TypeString = "Electric";
			break;
		case PokemonType::ICE:
			TypeString = "Ice";
			break;
		case PokemonType::FIGHTING:
			TypeString = "Fighting";
			break;
		case PokemonType::POISON:
			TypeString = "Poison";
			break;
		case PokemonType::GROUND:
			TypeString = "Ground";
			break;
		case PokemonType::FLYING:
			TypeString = "Flying";
			break;
		case PokemonType::PSYCHIC:
			TypeString = "Psychic";
			break;
		case PokemonType::BUG:
			TypeString = "Bug";
			break;
		case PokemonType::ROCK:
			TypeString = "Rock";
			break;
		case PokemonType::GHOST:
			TypeString = "Ghost";
			break;
		case PokemonType::DRAGON:
			TypeString = "Dragon";
			break;
		case PokemonType::STEEL:
			TypeString = "Steel";
			break;
		case PokemonType::DARK:
			TypeString = "Dark";
			break;
		default:
			break;
		}
		TypeString_.push_back(TypeString);
		GameEngineRenderer* NewRenderer = CreateRenderer(TypeString + ".bmp", GetOrder(), RenderPivot::LeftTop, { 660, 204 });
		NewRenderer->SetTransColor(RGB(255, 0, 255));
		NewRenderer->Off();
		PokemonTypeRenderer_.push_back(NewRenderer);
	}

	//========================================================= 포켓몬 스킬========================================================================

	//스킬 타입 렌더러
	for (size_t i = 0; i < PokemonInfoList_.size(); i++)
	{
		std::vector<PokemonSkill*> CurSkillList_ = PokemonInfoList_[i]->GetSkill();
		for (size_t j = 0; j<CurSkillList_.size();j++)
		{
			PokemonSkillInfo* CurSkillInfo = CurSkillList_[j]->GetInfo();

			std::string TypeString;
			PokemonType Type = CurSkillInfo->GetType();
			switch (Type)
			{
			case PokemonType::NORMAL:
				TypeString = "Normal";
				break;
			case PokemonType::FIRE:
				TypeString = "Fire";
				break;
			case PokemonType::WATER:
				TypeString = "Water";
				break;
			case PokemonType::GRASS:
				TypeString = "Grass";
				break;
			case PokemonType::ELECTRIC:
				TypeString = "Electric";
				break;
			case PokemonType::ICE:
				TypeString = "Ice";
				break;
			case PokemonType::FIGHTING:
				TypeString = "Fighting";
				break;
			case PokemonType::POISON:
				TypeString = "Poison";
				break;
			case PokemonType::GROUND:
				TypeString = "Ground";
				break;
			case PokemonType::FLYING:
				TypeString = "Flying";
				break;
			case PokemonType::PSYCHIC:
				TypeString = "Psychic";
				break;
			case PokemonType::BUG:
				TypeString = "Bug";
				break;
			case PokemonType::ROCK:
				TypeString = "Rock";
				break;
			case PokemonType::GHOST:
				TypeString = "Ghost";
				break;
			case PokemonType::DRAGON:
				TypeString = "Dragon";
				break;
			case PokemonType::STEEL:
				TypeString = "Steel";
				break;
			case PokemonType::DARK:
				TypeString = "Dark";
				break;
			default:
				break;
			}

			GameEngineRenderer* NewRenderer = CreateRenderer(TypeString + ".bmp", GetOrder(), RenderPivot::LeftTop, { 490,78+static_cast<float>(j*112  )});
			NewRenderer->SetTransColor(RGB(255, 0, 255));
			PokemonSkillTypeRenderer_[i].push_back(NewRenderer);
		}
	}


	//========================================================== 포켓몬스킬 셀렉트 ================================================================
	//타입
	for (size_t i = 0; i < PokemonInfoList_.size(); i++)
	{
		std::string& TypeString = TypeString_[i];
		GameEngineRenderer* NewRenderer = CreateRenderer(TypeString + ".bmp", GetOrder(), RenderPivot::LeftTop, { 180, 145 });
		NewRenderer->SetTransColor(RGB(255, 0, 255));
		NewRenderer->Off();
		PokemonSkillSelect_TypeRenderer_.push_back(NewRenderer);
	}

	//PokemonSkillSelect_SelectLineRenderer_
	//스킬 선택 빨간 외곽선
	for (size_t i = 0; i < PokemonInfoList_.size(); i++)
	{
		for (size_t j = 0; j < PokemonSkillTypeRenderer_[i].size(); j++)
		{
			GameEngineRenderer* NewRenderer = CreateRenderer("PokemonSkill_SelectFrame.bmp", GetOrder(), RenderPivot::LeftTop, { 480,72 + static_cast<float>(j * 112) });
			NewRenderer->SetTransColor(RGB(255, 0, 255));
			NewRenderer->Off();
			PokemonSkillSelect_SelectLineRenderer_[i].push_back(NewRenderer);
		}
		
	}


}

void PokemonSummaryMenu::InitFonts_()
{

	//--------------------------------------------------  포 켓 몬 인 포 ----------------------------------------------------------------------------
	//넘버 폰트
	{
		for (size_t i = 0; i < PokemonInfoList_.size(); i++)
		{
			GameEngineContentFont* NewFonts = GetLevel()->CreateActor<GameEngineContentFont>(GetOrder());
			NewFonts->SetPosition(GetPosition() + float4(660, 84));
			NewFonts->ShowString("00"+std::to_string(i), true);
			NewFonts->Off();
			NumberFonts_.push_back(NewFonts);
			AllFonts_.push_back(NewFonts);
		}
	}
	//레벨 폰트
	for (size_t i = 0; i < PokemonInfoList_.size(); i++)
	{
		{
			GameEngineContentFont* NewFonts = GetLevel()->CreateActor<GameEngineContentFont>(GetOrder());
			NewFonts->SetPosition(GetPosition()+float4(10,70));
			NewFonts->ShowString("Lv " + std::to_string(PokemonInfoList_[i]->GetMyLevel()), true);
			NewFonts->Off();
			LevelFonts_.push_back(NewFonts);
			AllFonts_.push_back(NewFonts);
		}
	}
	//포켓몬 프레임에 있는 이름 폰트
	for (size_t i = 0; i < PokemonInfoList_.size(); i++)
	{
		{
			GameEngineContentFont* NewFonts = GetLevel()->CreateActor<GameEngineContentFont>(GetOrder());
			NewFonts->SetPosition(GetPosition() + float4(180, 70));
			NewFonts->ShowString(PokemonInfoList_[i]->GetNameCopy(), true);
			NewFonts->Off();
			FrameNameFonts_.push_back(NewFonts);
			AllFonts_.push_back(NewFonts);
		}
	}
	//포켓몬 이름 폰트
	for (size_t i = 0; i < PokemonInfoList_.size(); i++)
	{
		{
			GameEngineContentFont* NewFonts = GetLevel()->CreateActor<GameEngineContentFont>(GetOrder());
			NewFonts->SetPosition(GetPosition() + float4(660, 140));
			NewFonts->ShowString(PokemonInfoList_[i]->GetNameCopy(), true);
			NewFonts->Off();
			NameFonts_.push_back(NewFonts);
			AllFonts_.push_back(NewFonts);
		}
	}

	//트레이너 이름 폰트
	{
		Owner_ = GetLevel()->CreateActor<GameEngineContentFont>(GetOrder());
		Owner_->SetPosition(GetPosition() + float4(660, 260));
		Owner_->ShowString("Red", true);
		AllFonts_.push_back(Owner_);
	}

	//ID넘버 폰트
	{
		IDNumber_ = GetLevel()->CreateActor<GameEngineContentFont>(GetOrder());
		IDNumber_->SetPosition(GetPosition() + float4(660, 320));
		IDNumber_->ShowString("15223", true);
		AllFonts_.push_back(IDNumber_);
	}

	//아이템 폰트
	{
		Item_ = GetLevel()->CreateActor<GameEngineContentFont>(GetOrder());
		Item_->SetPosition(GetPosition() + float4(660, 380));
		Item_->ShowString("None", true);
		AllFonts_.push_back(Item_);
	}

	//성격 폰트
	{
		for (size_t i = 0; i < PokemonInfoList_.size(); i++)
		{
			GameEngineContentFont* NewFonts = GetLevel()->CreateActor<GameEngineContentFont>(GetOrder());
			NewFonts->SetPosition(GetPosition() + float4(30, 460));
			NewFonts->ShowString(PokemonInfoList_[i]->GetNature()+" nature.", true);
			NewFonts->Off();
			NatureFonts_.push_back(NewFonts);
			AllFonts_.push_back(NewFonts);
		}
	}

	// =====================================================  포  켓  몬  어  빌  리  티 ========================================================================================
	//현재 Hp 폰트
	{
		for (size_t i = 0; i < PokemonInfoList_.size(); i++)
		{
			GameEngineContentFont* NewFonts = GetLevel()->CreateActor<GameEngineContentFont>(GetOrder());
			NewFonts->SetPosition(GetPosition() + float4(700, 80));
			NewFonts->ShowString(std::to_string(PokemonInfoList_[i]->GetHp())+"  /", true);
			NewFonts->Off();
			CurHpFonts_.push_back(NewFonts);
			AllFonts_.push_back(NewFonts);
		}
	}

	//최대 Hp 폰트
	{
		for (size_t i = 0; i < PokemonInfoList_.size(); i++)
		{
			GameEngineContentFont* NewFonts = GetLevel()->CreateActor<GameEngineContentFont>(GetOrder());
			NewFonts->SetPosition(GetPosition() + float4(870, 80));
			NewFonts->ShowString(std::to_string(PokemonInfoList_[i]->GetMaxHp()), true);
			NewFonts->Off();
			MaxHpFonts_.push_back(NewFonts);
			AllFonts_.push_back(NewFonts);
		}
	}

	//공격력 폰트
	{
		for (size_t i = 0; i < PokemonInfoList_.size(); i++)
		{
			GameEngineContentFont* NewFonts = GetLevel()->CreateActor<GameEngineContentFont>(GetOrder());
			NewFonts->SetPosition(GetPosition() + float4(860, 152));
			NewFonts->ShowString(std::to_string(PokemonInfoList_[i]->GetAtt()), true);
			NewFonts->Off();
			AttFonts_.push_back(NewFonts);
			AllFonts_.push_back(NewFonts);
		}
	}

	//방어력 폰트
	{
		for (size_t i = 0; i < PokemonInfoList_.size(); i++)
		{
			GameEngineContentFont* NewFonts = GetLevel()->CreateActor<GameEngineContentFont>(GetOrder());
			NewFonts->SetPosition(GetPosition() + float4(860, 204));
			NewFonts->ShowString(std::to_string(PokemonInfoList_[i]->GetDef()), true);
			NewFonts->Off();
			DefFonts_.push_back(NewFonts);
			AllFonts_.push_back(NewFonts);
		}
	}

	//특공 폰트
	{
		for (size_t i = 0; i < PokemonInfoList_.size(); i++)
		{
			GameEngineContentFont* NewFonts = GetLevel()->CreateActor<GameEngineContentFont>(GetOrder());
			NewFonts->SetPosition(GetPosition() + float4(860, 256));
			NewFonts->ShowString(std::to_string(PokemonInfoList_[i]->GetSpAtt()), true);
			NewFonts->Off();
			SpAttFonts_.push_back(NewFonts);
			AllFonts_.push_back(NewFonts);
		}
	}

	//특방 폰트
	{
		for (size_t i = 0; i < PokemonInfoList_.size(); i++)
		{
			GameEngineContentFont* NewFonts = GetLevel()->CreateActor<GameEngineContentFont>(GetOrder());
			NewFonts->SetPosition(GetPosition() + float4(860, 308));
			NewFonts->ShowString(std::to_string(PokemonInfoList_[i]->GetSpDef()), true);
			NewFonts->Off();
			SpDefFonts_.push_back(NewFonts);
			AllFonts_.push_back(NewFonts);
		}
	}

	//스피드 폰트
	{
		for (size_t i = 0; i < PokemonInfoList_.size(); i++)
		{
			GameEngineContentFont* NewFonts = GetLevel()->CreateActor<GameEngineContentFont>(GetOrder());
			NewFonts->SetPosition(GetPosition() + float4(860, 360));
			NewFonts->ShowString(std::to_string(PokemonInfoList_[i]->GetSpeed()), true);
			NewFonts->Off();
			SpeedFonts_.push_back(NewFonts);
			AllFonts_.push_back(NewFonts);
		}
	}

	//현재 Exp 폰트
	{
		for (size_t i = 0; i < PokemonInfoList_.size(); i++)
		{
			GameEngineContentFont* NewFonts = GetLevel()->CreateActor<GameEngineContentFont>(GetOrder());
			NewFonts->SetPosition(GetPosition() + float4(860, 416));
			NewFonts->ShowString(std::to_string(PokemonInfoList_[i]->GetExp()), true);
			NewFonts->Off();
			CurExpFonts_.push_back(NewFonts);
			AllFonts_.push_back(NewFonts);
		}
	}

	//최대 Exp 폰트
	{
		for (size_t i = 0; i < PokemonInfoList_.size(); i++)
		{
			GameEngineContentFont* NewFonts = GetLevel()->CreateActor<GameEngineContentFont>(GetOrder());
			NewFonts->SetPosition(GetPosition() + float4(850, 464));
			NewFonts->ShowString(std::to_string(PokemonInfoList_[i]->GetMaxExp()- PokemonInfoList_[i]->GetExp()), true);
			NewFonts->Off();
			NextExpFonts_.push_back(NewFonts);
			AllFonts_.push_back(NewFonts);
		}
	}

	//어빌리티 폰트
	{
		for (size_t i = 0; i < PokemonInfoList_.size(); i++)
		{
			GameEngineContentFont* NewFonts = GetLevel()->CreateActor<GameEngineContentFont>(GetOrder());
			NewFonts->SetPosition(GetPosition() + float4(300, 516));
			NewFonts->ShowString("Torrent", true);
			NewFonts->Off();
			AbilityFonts_.push_back(NewFonts);
			AllFonts_.push_back(NewFonts);
		}
	}
	//어빌리티 설명 폰트
	{
		for (size_t i = 0; i < PokemonInfoList_.size(); i++)
		{
			GameEngineContentFont* NewFonts = GetLevel()->CreateActor<GameEngineContentFont>(GetOrder());
			NewFonts->SetPosition(GetPosition() + float4(30, 564));
			NewFonts->ShowString("Ups Power moves in a pinch.", true);
			NewFonts->Off();
			AbilityExplanationFonts_.push_back(NewFonts);
			AllFonts_.push_back(NewFonts);
		}
	}

	//=================================================================== Pokemon Skill ==========================================================
	//스킬 이름
	{
		for (size_t i = 0; i < PokemonInfoList_.size(); i++)
		{
			std::vector<PokemonSkill*> CurSkillList_ = PokemonInfoList_[i]->GetSkill();
			for (size_t j = 0; j < CurSkillList_.size(); j++)
			{
				GameEngineContentFont* NewFonts = GetLevel()->CreateActor<GameEngineContentFont>(GetOrder());
				NewFonts->SetPosition(GetPosition() + float4(640, 76 + static_cast<float>(j * 112)));
				NewFonts->ShowString(CurSkillList_[j]->GetInfo()->GetNameCopy(), true);
				NewFonts->Off();
				SkillNameFonts_[i].push_back(NewFonts);
				AllFonts_.push_back(NewFonts);
			}
		}
	}


	//스킬 현재 PP
	{
		for (size_t i = 0; i < PokemonInfoList_.size(); i++)
		{
			std::vector<PokemonSkill*> CurSkillList_ = PokemonInfoList_[i]->GetSkill();
			for (size_t j = 0; j < CurSkillList_.size(); j++)
			{
				GameEngineContentFont* NewFonts = GetLevel()->CreateActor<GameEngineContentFont>(GetOrder());
				NewFonts->SetPosition(GetPosition() + float4(700, 128 + static_cast<float>(j * 112)));
				NewFonts->ShowString("PP" + std::to_string(CurSkillList_[j]->GetInfo()->GetPP())+" /", true);
				NewFonts->Off();
				CurSkillPPFonts_[i].push_back(NewFonts);
				AllFonts_.push_back(NewFonts);
			}
		}
	}

	//스킬 맥스 PP
	{
		for (size_t i = 0; i < PokemonInfoList_.size(); i++)
		{
			std::vector<PokemonSkill*> CurSkillList_ = PokemonInfoList_[i]->GetSkill();
			for (size_t j = 0; j < CurSkillList_.size(); j++)
			{
				GameEngineContentFont* NewFonts = GetLevel()->CreateActor<GameEngineContentFont>(GetOrder());
				NewFonts->SetPosition(GetPosition() + float4(868, 128 + static_cast<float>(j * 112)));
				NewFonts->ShowString(std::to_string(CurSkillList_[j]->GetInfo()->GetMaxPP()), true);
				NewFonts->Off();
				MaxSkillPPFonts_[i].push_back(NewFonts);
				AllFonts_.push_back(NewFonts);
			}
		}
	}

	//===================================== 포켓몬 스킬 셀렉트 ======================================================================================
	//스킬의 벨류값(파워)
	{
		for (size_t i = 0; i < PokemonInfoList_.size(); i++)
		{
			std::vector<PokemonSkill*> CurSkillList_ = PokemonInfoList_[i]->GetSkill();
			for (size_t j = 0; j < CurSkillList_.size(); j++)
			{
				GameEngineContentFont* NewFonts = GetLevel()->CreateActor<GameEngineContentFont>(GetOrder());
				NewFonts->SetPosition(GetPosition() + float4(230, 228));
				NewFonts->ShowString(std::to_string(CurSkillList_[j]->GetInfo()->GetValue()), true);
				NewFonts->Off();
				SkillValueFonts_[i].push_back(NewFonts);
				AllFonts_.push_back(NewFonts);
			}
		}
	}

	//스킬의 정확도
	{
		for (size_t i = 0; i < PokemonInfoList_.size(); i++)
		{
			std::vector<PokemonSkill*> CurSkillList_ = PokemonInfoList_[i]->GetSkill();
			for (size_t j = 0; j < CurSkillList_.size(); j++)
			{
				GameEngineContentFont* NewFonts = GetLevel()->CreateActor<GameEngineContentFont>(GetOrder());
				NewFonts->SetPosition(GetPosition() + float4(230, 284));
				NewFonts->ShowString("100", true);
				NewFonts->Off();
				SkillAccFonts_[i].push_back(NewFonts);
				AllFonts_.push_back(NewFonts);
			}
		}
	}

	//스킬의 설명
	{
		for (size_t i = 0; i < PokemonInfoList_.size(); i++)
		{
			std::vector<PokemonSkill*> CurSkillList_ = PokemonInfoList_[i]->GetSkill();
			for (size_t j = 0; j < CurSkillList_.size(); j++)
			{
				GameEngineContentFont* NewFonts = GetLevel()->CreateActor<GameEngineContentFont>(GetOrder());
				NewFonts->SetPosition(GetPosition() + float4(20, 390));
				NewFonts->ShowString(CurSkillList_[j]->GetInfo()->GetDesc(), true);
				NewFonts->Off();
				SkillExplanationFonts_[i].push_back(NewFonts);
				AllFonts_.push_back(NewFonts);
			}
		}
	}
}

void PokemonSummaryMenu::DestroyPokemonSummaryMenu()
{
	for (GameEngineContentFont* i : AllFonts_)
	{
		i->ClearCurrentFonts();
	}
}

void PokemonSummaryMenu::ChangeState(PokemonSummaryMenuType _State)
{
	if (_State == CurState_)
	{
		return;
	}
	switch (_State)
	{
	case PokemonSummaryMenu::PokemonSummaryMenuType::PokemonInfo:
		PokemonInfoStart();
		break;
	case PokemonSummaryMenu::PokemonSummaryMenuType::PokemonAbility:
		PokemonAbilityStart();
		break;
	case PokemonSummaryMenu::PokemonSummaryMenuType::PokemonSkill:
		PokemonSkillStart();
		break;
	case PokemonSummaryMenu::PokemonSummaryMenuType::PokemonSkillSelect:
		PokemonSkillSelectStart();
		break;
	default:
		break;
	}

	CurState_ = _State;
}

void PokemonSummaryMenu::UpdateState()
{
	switch (CurState_)
	{
	case PokemonSummaryMenu::PokemonSummaryMenuType::PokemonInfo:
		PokemonInfoUpdate();
		break;
	case PokemonSummaryMenu::PokemonSummaryMenuType::PokemonAbility:
		PokemonAbilityUpdate();
		break;
	case PokemonSummaryMenu::PokemonSummaryMenuType::PokemonSkill:
		PokemonSkillUpdate();
		break;
	case PokemonSummaryMenu::PokemonSummaryMenuType::PokemonSkillSelect:
		PokemonSkillSelectUpdate();
		break;
	default:
		break;
	}
}

void PokemonSummaryMenu::PokemonInfoStart()
{

}

void PokemonSummaryMenu::PokemonInfoUpdate()
{
	if (GameEngineInput::GetInst()->IsDown("X") == true)
	{
		WorldMapSoundManager::GetInst()->PlayEffectSound(WorldSoundEffectEnum::Click);
		DestroyPokemonSummaryMenu();
		Off();
	}

	if (GameEngineInput::GetInst()->IsDown("Up") == true)
	{
		if (CurrentOrder_ <= 0)
		{
			return;
		}
		WorldMapSoundManager::GetInst()->PlayEffectSound(WorldSoundEffectEnum::Click);
		PokemonJumpStart();
		CurrentOrder_--;
	}

	if (GameEngineInput::GetInst()->IsDown("Down") == true)
	{
		if (CurrentOrder_ >= PokemonInfoList_.size() - 1)
		{
			return;
		}
		WorldMapSoundManager::GetInst()->PlayEffectSound(WorldSoundEffectEnum::Click);
		PokemonJumpStart();
		CurrentOrder_++;
	}


	if (GameEngineInput::GetInst()->IsDown("Right") == true)
	{
		WorldMapSoundManager::GetInst()->PlayEffectSound(WorldSoundEffectEnum::Click);
		ChangeState(PokemonSummaryMenuType::PokemonAbility);
		return;
	}
}

void PokemonSummaryMenu::PokemonAbilityStart()
{


}

void PokemonSummaryMenu::PokemonAbilityUpdate()
{
	if (GameEngineInput::GetInst()->IsDown("Up") == true)
	{
		if (CurrentOrder_ <= 0)
		{
			return;
		}
		WorldMapSoundManager::GetInst()->PlayEffectSound(WorldSoundEffectEnum::Click);
		PokemonJumpStart();
		CurrentOrder_--;
	}

	if (GameEngineInput::GetInst()->IsDown("Down") == true)
	{
		if (CurrentOrder_ >= PokemonInfoList_.size() - 1)
		{
			return;
		}
		WorldMapSoundManager::GetInst()->PlayEffectSound(WorldSoundEffectEnum::Click);
		PokemonJumpStart();
		CurrentOrder_++;
	}


	if (GameEngineInput::GetInst()->IsDown("Left") == true)
	{
		WorldMapSoundManager::GetInst()->PlayEffectSound(WorldSoundEffectEnum::Click);
		ChangeState(PokemonSummaryMenuType::PokemonInfo);
		return;
	}

	if (GameEngineInput::GetInst()->IsDown("Right") == true)
	{
		WorldMapSoundManager::GetInst()->PlayEffectSound(WorldSoundEffectEnum::Click);
		ChangeState(PokemonSummaryMenuType::PokemonSkill);
		return;
	}

	if (GameEngineInput::GetInst()->IsDown("X") == true)
	{
		WorldMapSoundManager::GetInst()->PlayEffectSound(WorldSoundEffectEnum::Click);
		DestroyPokemonSummaryMenu();
		Off();
	}
}

void PokemonSummaryMenu::PokemonSkillStart()
{
}

void PokemonSummaryMenu::PokemonSkillUpdate()
{
	if (GameEngineInput::GetInst()->IsDown("Left") == true)
	{
		WorldMapSoundManager::GetInst()->PlayEffectSound(WorldSoundEffectEnum::Click);
		ChangeState(PokemonSummaryMenuType::PokemonAbility);
		return;
	}

	if (GameEngineInput::GetInst()->IsDown("Z") == true)
	{
		WorldMapSoundManager::GetInst()->PlayEffectSound(WorldSoundEffectEnum::Click);
		ChangeState(PokemonSummaryMenuType::PokemonSkillSelect);
		return;
	}

	if (GameEngineInput::GetInst()->IsDown("X") == true)
	{
		WorldMapSoundManager::GetInst()->PlayEffectSound(WorldSoundEffectEnum::Click);
		DestroyPokemonSummaryMenu();
		Off();
		return;
	}

	if (GameEngineInput::GetInst()->IsDown("Up") == true)
	{
		if (CurrentOrder_ <= 0)
		{
			return;
		}
		WorldMapSoundManager::GetInst()->PlayEffectSound(WorldSoundEffectEnum::Click);
		PokemonJumpStart();
		CurrentOrder_--;
	}

	if (GameEngineInput::GetInst()->IsDown("Down") == true)
	{
		if (CurrentOrder_ >= PokemonInfoList_.size() - 1)
		{
			return;
		}
		WorldMapSoundManager::GetInst()->PlayEffectSound(WorldSoundEffectEnum::Click);
		PokemonJumpStart();
		CurrentOrder_++;
	}
}

void PokemonSummaryMenu::PokemonSkillSelectStart()
{

}

void PokemonSummaryMenu::PokemonSkillSelectUpdate()
{
	if (GameEngineInput::GetInst()->IsDown("X") == true)
	{
		WorldMapSoundManager::GetInst()->PlayEffectSound(WorldSoundEffectEnum::Click);
		ChangeState(PokemonSummaryMenuType::PokemonSkill);
		SkillSelect_CurrentOrder_ = 0;
		return;
	}

	if (GameEngineInput::GetInst()->IsDown("Up") == true)
	{
		WorldMapSoundManager::GetInst()->PlayEffectSound(WorldSoundEffectEnum::Click);
		if (SkillSelect_CurrentOrder_ <= 0)
		{
			SkillSelect_CurrentOrder_ = static_cast<int>(PokemonSkillTypeRenderer_[CurrentOrder_].size()) - 1;
			return;
		}
		SkillSelect_CurrentOrder_--;
	}

	if (GameEngineInput::GetInst()->IsDown("Down") == true)
	{
		WorldMapSoundManager::GetInst()->PlayEffectSound(WorldSoundEffectEnum::Click);
		if (SkillSelect_CurrentOrder_ >= PokemonSkillTypeRenderer_[CurrentOrder_].size() - 1)
		{
			SkillSelect_CurrentOrder_ = 0;
			return;
		}
		SkillSelect_CurrentOrder_++;
	}
}

void PokemonSummaryMenu::PokemonJump()
{
	if (IsJumpStart_ == true)
	{
		JumpTime_ += GameEngineTime::GetDeltaTime();
		if (JumpTime_ > 0.5f)
		{
			CurYPos_ = 0;
			AccGravity_ = 0;
			JumpTime_ = 0;
			IsJumpStart_ = false;
		}

		CurYPos_ += AccGravity_ *GameEngineTime::GetDeltaTime();

		if (CurYPos_ > 20.0f)
		{
			AccGravity_ -= 10000 * GameEngineTime::GetDeltaTime();
		}
		else
		{
			AccGravity_ += 10000 * GameEngineTime::GetDeltaTime();
		}

		PokemonFrontRenderer_[CurrentOrder_]->SetPivot({ 120,120 - CurYPos_ });
	}
	else
	{
		PokemonFrontRenderer_[CurrentOrder_]->SetPivot({ 120,120 });
	}
}

void PokemonSummaryMenu::PokemonJumpStart()
{
	IsJumpStart_ = true;
	PokemonFrontRenderer_[CurrentOrder_]->SetPivot({ 120,120 });
	CurYPos_ = 0;
	AccGravity_ = 0;
	JumpTime_ = 0;
}
