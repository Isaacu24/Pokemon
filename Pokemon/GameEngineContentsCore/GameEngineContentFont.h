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
	// True  : ��� ���� ���������� �Ϸ�
	// False : �̹� ��ϵ� ��� ����� ����
	//
	// _String : ����� ����
	// _LineHeight : �� �� ����
	// _DelayTime : ���� �� ��� ������
	// _Order : Renderer Order
	bool ShowString(const std::string& _String, float _LineHeight = 60.0f, float _DelayTime = 0.02f, int _Order = 10);


	// ��� �Ϸ� �� Ű �Է� ��� ������ Ȯ���ϴ� �Լ�
	//
	// True  : �Է� ��� ��
	// False : ��� ���̰ų� �Է� ������� �ƴ�
	inline bool IsWattingKeyPush() const
	{
		return WatingKeyPush_;
	}


	// ȭ�� �� �ش� �������̽��� �������ִ� ��Ʈ ����
	inline void ClearFonts()
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
		WatingKeyPush_ = false;
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
	float LineHeight_;
	//
	// Order
	int Order_;

	// Ű Ǫ�� ���
	bool WatingKeyPush_;
	//


	std::queue<std::string> StringQueue_;

	std::string CurrentString_;
	std::string CurrentWord_;
	std::vector<GameEngineRenderer*> AllFontRenderer_;
	// static std::list<GameEngineContentFont*> AllFonts_;
};