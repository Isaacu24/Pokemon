#pragma once
#include "PokemonTileMap.h"
#include "Room4Pokeball.h"
#include <GameEngine/GameEngineRenderer.h>

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

	Room4Pokeball* Pokeball0;	// �̻��ؾ�
	Room4Pokeball* Pokeball1;	// ���α�
	Room4Pokeball* Pokeball2;	// ���̸�
protected:

private:
	GameEngineRenderer* FrontRenderer_;
	void Start() override;

};

