#include "BattleInerface.h"

BattleInerface::BattleInerface()
{

}

BattleInerface::~BattleInerface()
{

}

void BattleInerface::Start()
{
	InterfaceImage = CreateRenderer("Battle_Select.bmp",0);
	
	Select = CreateRenderer("Select.bmp", 1);

	//Select->SetPivot({ -190.0f, -25.0f });//Fight�� ������������
}

void BattleInerface::Render()
{
}

void BattleInerface::Update()
{
	
}
