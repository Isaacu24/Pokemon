#pragma once
#include "PokemonInfo.h"
#include "PokemonInfoManager.h"

class Pokemon
{
public:
	Pokemon();
	~Pokemon();

	Pokemon(const Pokemon& _Other) = delete;
	Pokemon(Pokemon&& _Other) noexcept = delete;
	Pokemon& operator=(const Pokemon& _Other) = delete;
	Pokemon& operator=(Pokemon&& _Other) noexcept = delete;

	inline PokemonInfo* GetInfo()
	{
		return &MyInfo_;
	}


protected:


private:
	PokemonInfo MyInfo_;
};

