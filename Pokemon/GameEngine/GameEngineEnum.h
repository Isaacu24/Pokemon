#pragma once

enum class RenderPivot
{
	LeftTop,
	CENTER,
	BOT,
};

enum class RenderScaleMode
{
	Image,
	User,
};

enum class CollisionType
{
	Point,
	Circle,
	Rect,
	Max
};

enum class EngineMax
{
	RENDERORDERMAX = 2147483647,
};
