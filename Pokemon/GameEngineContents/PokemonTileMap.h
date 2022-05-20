#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineRendererTileMap.h>
#include <GameEngine/GameEngineImage.h>
#include <GameEngine/GameEngineImageManager.h>

enum class TileState
{
	False,
	True,
	MoreDown,
};

// ���� :
class PokemonTileMap : public GameEngineActor
{
public:
	// constrcuter destructer
	PokemonTileMap();
	~PokemonTileMap();

	// delete Function
	PokemonTileMap(const PokemonTileMap& _Other) = delete;
	PokemonTileMap(PokemonTileMap&& _Other) noexcept = delete;
	PokemonTileMap& operator=(const PokemonTileMap& _Other) = delete;
	PokemonTileMap& operator=(PokemonTileMap&& _Other) noexcept = delete;

	float4 GetWorldPostion(int _X, int _Y);
	inline GameEngineRendererTileMap& GetTileMap()
	{
		return TileMap_;
	}
	inline TileState CanMove(int _X, int _Y, float4 _Dir = float4::ZERO)
	{
		// Ÿ�ϸ��� �������
		if (TileMap_.GetMaxX() <= _X || TileMap_.GetMaxY() <= _Y)
		{
			return TileState::False;
		}

		float4 TestPos = TileMap_.GetWorldPostion(_X, _Y);
		int color = ColMap_->GetImagePixel(TestPos);
		if (RGB(0, 0, 0) != color && RGB(0, 255, 0) != color)
		{
			// �ش� ��ǥ�� �ݸ��� ������, �ʷϻ��� �ƴҶ�
			return TileState::True;
		}
		else if (RGB(0, 255, 0) == color)
		{
			// �ش� ��ǥ�� �ݸ��� �ʷϻ��϶�
			_Dir.Normal2D();
			if (_Dir.CompareInt2D(float4::DOWN))
			{
				// �������°Ÿ� ��ĭ�� ������
				return TileState::MoreDown;
			}
		}

		return TileState::False;
	}

protected:
	GameEngineRendererTileMap TileMap_;
	GameEngineImage* ColMap_;

private:

};

