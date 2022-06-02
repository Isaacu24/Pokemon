#pragma once
#include <GameEngineBase/GameEngineTime.h>
#include <GameEngineBase/GameEngineMath.h>
#include <GameEngine/GameEngineRenderer.h>
#include <GameEngine/GameEngineActor.h>
#include <list>
#include <string>
#include <queue>

// ���� :
class GameEngineContentFont : public GameEngineActor
{
public:
	// constrcuter destructer
	GameEngineContentFont();
	~GameEngineContentFont();

	// delete Function
	GameEngineContentFont(const GameEngineContentFont& _Other) = delete;
	GameEngineContentFont(GameEngineContentFont&& _Other) noexcept = delete;
	GameEngineContentFont& operator=(const GameEngineContentFont& _Other) = delete;
	GameEngineContentFont& operator=(GameEngineContentFont&& _Other) noexcept = delete;

	// �ؽ�Ʈ ��� ��� �Լ�
	//
	// ���پ� ����ϴ� ���� ����
	// 
	// True  : ��� ���� ���������� �Ϸ�
	// False : �̹� ��ϵ� ��� ����� ����
	//
	// _String : ����� ����	
	// _ShowAllText : �ִϸ��̼� ���·� ��� �Ϸ��� false, �ѹ��� ����Ϸ��� true
	// _LineHeight : �� �� ����
	// _DelayTime : ���� �� ��� ������
	// _Order : Renderer Order
	bool ShowString(const std::string& _String, bool _ShowAllText = false, float _LineHeight = 60.0f, float _DelayTime = 0.02f);


	// �ؽ�Ʈ ��� �ִϸ��̼��� ������Ű�� ������ �̿��ϴ� �Լ�
	// 
	// ȣ�� �� ��� �ؽ�Ʈ ���� ��� ȣ��ȴ� (��, �� �� ����)
	inline void SkipCharAnimation()
	{
		ShowAllText_ = true;
	}

	// ��� �Ϸ� �� Ű �Է� ��� ������ Ȯ���ϴ� �Լ�
	//
	// True  : �Է� ��� ��
	// False : ��� ���̰ų� �Է� ������� �ƴ�
	inline bool IsWait() const
	{
		return (WatingKeyPush_ && !StringQueue_.empty());
	}

	// ��� ������ ����� �Ǿ����� Ȯ���ϴ� �Լ�
	//
	// True	 : ��� ���� ��� �Ϸ�
	// False : ���� ��⿭�� �ִ� ������ ����
	inline bool IsEnd() const
	{
		return (StringQueue_.empty() && WatingKeyPush_);
	}

	// ȭ�鿡 ��µ� ��Ʈ ���� �� ���� ���� ���
	// 
	// True  : ���� ȭ�鿡 �׷��� ������ ����� ���� ���� ���
	// False : ���� ������ ���ų� �̹� �����
	bool NextString()
	{
		if (!StringQueue_.empty() && WatingKeyPush_ == true)
		{
			ClearCurrentFonts();
			WatingKeyPush_ = false;
			return true;
		}
		else
		{
			return false;
		}
	}

	// ȭ�� �� ��µ� ��Ʈ ����
	// 
	// 
	// NextString�� ClearFonts ������
	// 
	// �ٷ� ���� ������ ����ϰ� �ʹ� 
	// -> NextString(ȭ�鿡 ��µ� ��Ʈ�� ����� ���� ���� ���)
	// 
	// � ����̳� �׼�, �ִϸ��̼��� ����ϰ� �����ڿ� ������ ����ϰ�ʹ�
	//
	// -> ClearFonts(ȭ��� ��µ� ��Ʈ�� �����) 
	// -> �׼� Ȥ�� �ִϸ��̼� ��� 
	// -> NextString(ȭ�鿡 ��µ� ��Ʈ ����� ���� ���� ����ε�, ȭ��� ��µ� ��Ʈ�� �̹� �������� �׳� ���� ���� ���)
	//
	void ClearCurrentFonts()
	{
		for (auto Iter : AllFontRenderer_)
		{
			if (Iter != nullptr)
			{
				Iter->Death();
				Iter = nullptr;
			}
		}
		AllFontRenderer_.clear();
	}

	// ��� ���̰ų� ������� ��� ������� ����
	// ������, �������� �ʰ� ��� ������ �����մϴ�
	void EndFont()
	{
		for (auto Iter : AllFontRenderer_)
		{
			if (Iter != nullptr)
			{
				Iter->Death();
				Iter = nullptr;
			}
		}
		StringQueue_ = std::queue<std::string>();
		CurrentString_.clear();
		ShowAllText_ = false;
		WatingKeyPush_ = false;
		Off();
	}


	// ��Ʈ�� ��µǾ��ִ��� Ȯ���ϴ� �Լ�
	// true : ����
	// false : ����
	inline bool IsRendererFont()
	{
		if (AllFontRenderer_.empty())
		{
			return false;
		}
		else
		{
			return true;
		}

	}


	const std::string& GetCurrentString() 
	{
		return CurrentString_;
	}

protected:
	void Update() override;
	void Start() override;
	void Render() override {}

private:
	float4 CurrentPivot_;
	float OriginalWaitTime_;
	float CurrentWaitTime_;

	// ���� ��
	int CurrentStringRow_;

	// �� ����
	float LineHeight_;
	
	// Ű Ǫ�� ���
	bool WatingKeyPush_;

	// �ѹ��� ���
	bool ShowAllText_;


	std::queue<std::string> StringQueue_;

	std::string CurrentString_;
	std::vector<GameEngineRenderer*> AllFontRenderer_;
	// static std::list<GameEngineContentFont*> AllFonts_;
};