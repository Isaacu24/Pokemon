#include "GameEngineContentFont.h"
#include <GameEngineBase/GameEngineDebug.h>

GameEngineContentFont::GameEngineContentFont() 
	: CurrentPivot_(float4::ZERO)
	, OriginalWaitTime_(0)
	, CurrentWaitTime_(0)
	, CurrentStringRow_(0)
	, WatingKeyPush_(false)
	, LineHeight_(60.0f)
	, ShowAllText_(false)
{
}

GameEngineContentFont::~GameEngineContentFont() 
{

}

void GameEngineContentFont::Start()
{
	//std::list<GameEngineContentFont*> GameEngineContentFont::AllFonts_.push_back(this);
	Off();
}

void GameEngineContentFont::Update()
{
	if (StringQueue_.empty() == false && WatingKeyPush_ == false)
	{
		if (ShowAllText_ == true)
		{
			while (!StringQueue_.empty() && ShowAllText_ == true)
			{
				for (size_t i = 0; i < CurrentString_.size(); i++)
				{
					char CurrentWord = CurrentString_[i];
					std::string StringForRendererName = "";

					if (65 <= CurrentWord && CurrentWord <= 90)
					{
						StringForRendererName = CurrentWord;
						StringForRendererName += "_Upper.bmp";
					}
					else if (97 <= CurrentWord && CurrentWord <= 122)
					{
						StringForRendererName = CurrentWord;
						StringForRendererName += "_Lower.bmp";
					}
					else if (48 <= CurrentWord && CurrentWord <= 57)
					{
						StringForRendererName = CurrentWord;
						StringForRendererName += "_Number.bmp";
					}
					else
					{
						switch (CurrentWord)
						{
						case ' ':
							StringForRendererName = "LineJamp.bmp";
							break;
						case '?':
							StringForRendererName = "QuestionMark.bmp";
							break;
						case '!':
							StringForRendererName = "ExclamationMark.bmp";
							break;
						case 46:
							StringForRendererName = "Period.bmp";
							break;
						case '/':
							StringForRendererName = "Slash.bmp";
							break;
						case ';':
							StringForRendererName = "Ellipsis.bmp";
							break;
						case '\'':
							StringForRendererName = "Apostrophe_Right.bmp";
							break;
						case '\"':
							break;
						default:
							MsgBoxAssert("����Ҽ� ���� ���ڰ� ���ԵǾ��ֽ��ϴ� " + CurrentWord)
								break;
						}
					}

					GameEngineRenderer* CurrentRenderer = CreateRenderer(StringForRendererName, GetOrder(), RenderPivot::LeftTop, CurrentPivot_);
					CurrentPivot_ = { CurrentPivot_.x + CurrentRenderer->GetImageScale().x , static_cast<float>(LineHeight_ * CurrentStringRow_) };
					AllFontRenderer_.push_back(CurrentRenderer);

					if (CurrentString_[i+1] == 0)
					{
						++CurrentStringRow_;
						if (CurrentStringRow_ > 1)
						{
							// �Է� ���
							WatingKeyPush_ = true;
							CurrentStringRow_ = 0;
							ShowAllText_ = false;
						}
						CurrentPivot_ = { 10, static_cast<float>(60 * CurrentStringRow_) };
						StringQueue_.pop();
						// �Էµ� ��ȭ ��� ��¿Ϸ�
						if (StringQueue_.empty())
						{
							WatingKeyPush_ = true;
							ShowAllText_ = false;
						}
						// �ƴϸ� ���� ���� ���
						else
						{
							CurrentString_ = StringQueue_.front();
						}
						break;
					}
				}


			}
		}
		else
		{
			CurrentWaitTime_ -= GameEngineTime::GetDeltaTime(3);
			if (CurrentWaitTime_ < 0)
			{
				char CurrentWord = CurrentString_[0];
				std::string StringForRendererName = "";

				if (65 <= CurrentWord && CurrentWord <= 90)
				{
					StringForRendererName = CurrentWord;
					StringForRendererName += "_Upper.bmp";
				}
				else if (97 <= CurrentWord && CurrentWord <= 122)
				{
					StringForRendererName = CurrentWord;
					StringForRendererName += "_Lower.bmp";
				}
				else if (48 <= CurrentWord && CurrentWord <= 57)
				{
					StringForRendererName = CurrentWord;
					StringForRendererName += "_Number.bmp";
				}
				else
				{
					switch (CurrentWord)
					{
					case ' ':
						StringForRendererName = "LineJamp.bmp";
						break;
					case '?' :
						StringForRendererName = "QuestionMark.bmp";
						break;
					case '!' :
						StringForRendererName = "ExclamationMark.bmp";
						break;
					case 46:
						StringForRendererName = "Period.bmp";
						break;
					case '/':
						StringForRendererName = "Slash.bmp";
						break;
					case ';':
						StringForRendererName = "Ellipsis.bmp";
						break;
					case '\'':
						StringForRendererName = "Apostrophe_Right.bmp";
						break;
					case '\"':
						break;
					default:
						MsgBoxAssert("����Ҽ� ���� ���ڰ� ���ԵǾ��ֽ��ϴ� " + CurrentWord)
						break;
					}
				}

				CurrentString_.erase(0, 1);
				GameEngineRenderer* CurrentRenderer = CreateRenderer(StringForRendererName, GetOrder(), RenderPivot::LeftTop, CurrentPivot_);
				CurrentPivot_ = { CurrentPivot_.x + CurrentRenderer->GetImageScale().x , static_cast<float>(LineHeight_ * CurrentStringRow_)};
				AllFontRenderer_.push_back(CurrentRenderer);
				CurrentWaitTime_ = OriginalWaitTime_;

			}

			// �Ʒ��� ����ó��

			// �� �ٲ�
			if (CurrentString_.empty() == true )
			{
				++CurrentStringRow_;
				if (CurrentStringRow_ > 1)
				{
					// �Է� ���
					WatingKeyPush_ = true;
					CurrentStringRow_ = 0;
					ShowAllText_ = false;
				}
				CurrentPivot_ = { 10, static_cast<float>(60 * CurrentStringRow_)};
				StringQueue_.pop();
				// �Էµ� ��ȭ ��� ��¿Ϸ�
				if (StringQueue_.empty())
				{
					WatingKeyPush_ = true;
					ShowAllText_ = false;
				}
				else
				{
					CurrentString_ = StringQueue_.front();
				}
			}

		}
	}
}

bool GameEngineContentFont::ShowString(const std::string& _String, bool _ShowAllText, float _LineHeight /*= 60.0f*/, float _DelayTime /* = 0.02f*/)
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
	if (AllFontRenderer_.empty() == false)
	{
		ClearCurrentFonts();

	}

	ShowAllText_ = _ShowAllText;
	CurrentString_.clear();
	CurrentStringRow_ = 0;
	LineHeight_ = _LineHeight;
	OriginalWaitTime_ = _DelayTime;
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
			
			StringQueue_.push(GetString.substr(0, IdxCount));
			GetString.erase(0, IdxCount + 1);
			IdxCount = 0;
		}
		else
		{
			++IdxCount;
		}
	}
	CurrentString_ = StringQueue_.front();
	On();
	return true;
}


//void GameEngineContentFont::Destroy()
//{
//	//for (auto Font : AllFonts_)
//	//{
//	//	if (Font != nullptr)
//	//	{
//	//		delete Font;
//	//		Font = nullptr;
//	//	}
//	//}
//}