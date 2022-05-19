#include "PokemonTileMap.h"
#include <GameEngineBase/GameEngineDebug.h>

PokemonTileMap::PokemonTileMap() 
	: TileMap_(GameEngineRendererTileMap(this))
{
}

PokemonTileMap::~PokemonTileMap() 
{
}

float4 PokemonTileMap::GetWorldPostion(int _X, int _Y)
{
	//if (false == TileMap_.IsTile(_X, _Y))
	//{
	//	MsgBoxAssert("�������� �ʴ� Ÿ���� ��ġ�� �˷��� �߽��ϴ�.");
	//}

	return GetPosition() + TileMap_.GetWorldPostion(_X, _Y);
}