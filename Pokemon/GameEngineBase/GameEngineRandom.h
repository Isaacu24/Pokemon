#pragma once
#include <random>

// 00000000
// 10110010

// 제네레이터
// uniform 규격으로 

// 제네레이터 클래스가 있다.

// 유니폼 클래스가 있고

// 설명 :
class GameEngineRandom
{
	// 제네레이터
	// 메르헨 트위스터의 알고리즘으로 비트를 뒤섞는 녀석.
	std::mt19937_64 mt_;

	//std::uniform_int_distribution<int> Uniform;
public:
	static GameEngineRandom* GetInst_;
	static GameEngineRandom* GetRandom()
	{
		return GetInst_;
	}

	static void Destroy()
	{
		if (GetInst_ != nullptr)
		{
			delete GetInst_;
		}
	}
	// 
	int RandomInt(int _Min, int _Max)
	{
		std::uniform_int_distribution<int> Uniform(_Min, _Max);
		return Uniform(mt_);
	}

	float RandomFloat(float _Min, float _Max)
	{
		// 실수는 생성기가 Real
		std::uniform_real_distribution<float> Uniform(_Min, _Max);
		return Uniform(mt_);
	}



public:
	// constrcuter destructer
	GameEngineRandom();
	~GameEngineRandom();

	// delete Function
	GameEngineRandom(const GameEngineRandom& _Other) = delete;
	GameEngineRandom(GameEngineRandom&& _Other) noexcept = delete;
	GameEngineRandom& operator=(const GameEngineRandom& _Other) = delete;
	GameEngineRandom& operator=(GameEngineRandom&& _Other) noexcept = delete;

protected:

private:

};

