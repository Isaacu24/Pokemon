#pragma once

// ���� :
enum class Fonts;
class GameEngineContentFontManager
{
public:
	// constrcuter destructer
	GameEngineContentFontManager();
	~GameEngineContentFontManager();

	// delete Function
	GameEngineContentFontManager(const GameEngineContentFontManager& _Other) = delete;
	GameEngineContentFontManager(GameEngineContentFontManager&& _Other) noexcept = delete;
	GameEngineContentFontManager& operator=(const GameEngineContentFontManager& _Other) = delete;
	GameEngineContentFontManager& operator=(GameEngineContentFontManager&& _Other) noexcept = delete;

protected:

private:

};

enum class Fonts
{

};


