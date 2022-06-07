#pragma once
#include <GameEngine/GameEngineActor.h>

class FadeActor;
class PokemonInfo;
class GameEngineContentFont;
class MenuUI : public GameEngineActor
{
public:
	MenuUI();
	~MenuUI();


	MenuUI(const MenuUI& _Other) = delete;
	MenuUI(const MenuUI&& _Other) noexcept = delete;
	MenuUI& operator=(const MenuUI& _Ohter) = delete;
	MenuUI& operator=(const MenuUI&& _Other) noexcept = delete;

	void InitMenuUI();

protected:
	void Start() override;
	void Update() override;
	void Render() override;
private:
	bool IsOn_; // �̰� true�� �Ǿ� ���Ͱ� ������Ʈ

	int CurrentOrder_;
	std::vector<GameEngineRenderer*> MenuUIRenderer_;
	FadeActor* FadeActor_;
	GameEngineActor* ChildUI_;

};

