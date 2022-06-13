#pragma once
#include "PokemonTileMap.h"

// ���� : ���ڻ� ������
class RoomTileMap4 : public PokemonTileMap
{
private:
	static RoomTileMap4* Inst_;

public:
	static RoomTileMap4* GetInst()
	{
		return Inst_;
	}

public:
	// constrcuter destructer
	RoomTileMap4();
	~RoomTileMap4();

	// delete Function
	RoomTileMap4(const RoomTileMap4& _Other) = delete;
	RoomTileMap4(RoomTileMap4&& _Other) noexcept = delete;
	RoomTileMap4& operator=(const RoomTileMap4& _Other) = delete;
	RoomTileMap4& operator=(RoomTileMap4&& _Other) noexcept = delete;

protected:

private:
	void Start() override;

	GameEngineRenderer* PokeballRender0_;	// �̻��ؾ�
	GameEngineRenderer* PokeballRender1_;	// ���α�
	GameEngineRenderer* PokeballRender2_;	// ���̸�
};

