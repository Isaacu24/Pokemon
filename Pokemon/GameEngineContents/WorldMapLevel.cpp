#include "WorldMapLevel.h"
#include "WorldMapBackground.h"
#include "PlayerRed.h"
#include "WorldTileMap1.h"
#include "RoomTileMap1.h"

WorldMapLevel::WorldMapLevel() 
{
}

WorldMapLevel::~WorldMapLevel() 
{
}

void WorldMapLevel::Loading()
{
	CreateActor<WorldMapBackground>();
	CreateActor<WorldTileMap1>();
	CreateActor<RoomTileMap1>();
	CreateActor<PlayerRed>();
}

void WorldMapLevel::Update()
{
}

