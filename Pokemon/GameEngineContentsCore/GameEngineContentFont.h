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

	//void Destroy();

	// �ؽ�Ʈ ��� ��� �Լ�
	//
	// True  : ��� ���� ���������� �Ϸ�
	// False : �̹� ��ϵ� ��� ����� ����
	bool ShowString(const std::string& _String, float _LineHeight = 60.0f, float _WaitTime = 0.02f);
	
	// ���� ���� ��� �Լ�
	// 
	// True  : ��� �� (Ȥ�� ����)
	// False : ���� ������ ����
	inline bool NextString() 
	{ 
		return WatingKeyPush_; 
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
	// 
	// Ű Ǫ�� ���
	bool WatingKeyPush_;
	//


	std::queue<std::string> StringQueue_;

	std::string CurrentString_;
	std::string CurrentWord_;
	// static std::list<GameEngineContentFont*> AllFonts_;
};