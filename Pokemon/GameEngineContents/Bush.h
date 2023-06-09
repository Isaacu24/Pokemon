#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineRenderer.h>

// ���� :
class Bush : public GameEngineActor
{
public:
	// constrcuter destructer
	Bush();
	~Bush();

	// delete Function
	Bush(const Bush& _Other) = delete;
	Bush(Bush&& _Other) noexcept = delete;
	Bush& operator=(const Bush& _Other) = delete;
	Bush& operator=(Bush&& _Other) noexcept = delete;

	void CreateEffect();

protected:

private:
	GameEngineRenderer* Renderer_;
	float NextTimer_;

	void Start() override;
};

