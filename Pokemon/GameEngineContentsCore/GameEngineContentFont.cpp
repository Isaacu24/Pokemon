#include "GameEngineContentFont.h"
#include <GameEngineBase/GameEngineDebug.h>

GameEngineContentFont::GameEngineContentFont() 
	: CurrentPivot_(float4::ZERO)
	, OriginalWaitTime_(0)
	, CurrentWaitTime_(0)
	, StringRow_(0)
	, WatingKeyPush_(false)
{
}

GameEngineContentFont::~GameEngineContentFont() 
{

}

void GameEngineContentFont::Start()
{
	GameEngineContentFont::AllFonts_.push_back(this);
	Off();
}

void GameEngineContentFont::Update()
{
	if (StringQueue_.empty() == false && WatingKeyPush_ == false)
	{
		CurrentWaitTime_ -= GameEngineTime::GetDeltaTime(3);
		if (CurrentWaitTime_ < 0)
		{
			std::string CurrentWord = CurrentString_.erase();
			GameEngineRenderer* CurrentRenderer = CreateRenderer(CurrentWord, 10, RenderPivot::LeftTop, CurrentPivot_);
			CurrentPivot_ = {CurrentRenderer->GetImageScale().x, 30 * StringRow_};
			CurrentWaitTime_ = OriginalWaitTime_;
		}

		// �Ʒ��� ����ó��

		// �� �ٲ�
		if (CurrentString_.front() == '\\' || CurrentString_.empty() == true)
		{
			if (StringRow_ > 1)
			{
				// �Է� ���
				WatingKeyPush_ = true;
				
			}
			++StringRow_;
			StringQueue_.pop();
			CurrentString_ = StringQueue_.front();
		}
		// �Էµ� ��ȭ ��� ��¿Ϸ�
		else if (CurrentString_.front() == 0)
		{
			WatingKeyPush_ = true;
		}
	}
}

bool GameEngineContentFont::ShowString(const std::string& _String, float _WaitTime /* = 0.01f*/)
{
	if (GetPosition().CompareInt2D(float4::ZERO))
	{
		MsgBoxAssert("��Ʈ ����� ��ġ�� ������ �ȵǾ� �ֽ��ϴ�");
		return false;
	}

	if (StringQueue_.empty() == false)
	{
		return false;
	}


	StringRow_ = 0;
	OriginalWaitTime_ = _WaitTime;
	CurrentWaitTime_ = 0;
	std::string GetString = _String;
	int IdxCount= 0;

	// ��ġ�������� 10�ȼ� ������ ���� ���� ����
	CurrentPivot_ = { 10, 0 };

	WatingKeyPush_ = false;


	while (GetString.empty() == false)
	{
		if (GetString[IdxCount] == 0)
		{
			StringQueue_.push(GetString.substr(0, IdxCount));
			break;
		}
		if (GetString[IdxCount] == '\\')
		{
			StringQueue_.push(GetString.erase(0, IdxCount));
			IdxCount = 0;
		}
		else
		{
			++IdxCount;
		}
	}
	CurrentString_.clear();

	On;
}


void GameEngineContentFont::Destroy()
{
	for (auto Font : AllFonts_)
	{
		if (Font != nullptr)
		{
			delete Font;
			Font = nullptr;
		}
	}
}