#pragma once
#include <GameEngine/GameEngineActor.h>

// ���� :
class Item : public GameEngineActor
{
public:
	// constrcuter destructer
	Item();
	~Item();

	// delete Function
	Item(const Item& _Other) = delete;
	Item(Item&& _Other) noexcept = delete;
	Item& operator=(const Item& _Other) = delete;
	Item& operator=(Item&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update() override;

private:
	int Value; //��ó���� �� ȸ���� ��ġ

};

