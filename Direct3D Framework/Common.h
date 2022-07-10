#pragma once
#include "SimpleMath.h"

// 2ŸŒ³‚Ì“àÏ‚ğŒvZ‚·‚é
inline float Dot2D(const DirectX::SimpleMath::Vector2& a, const DirectX::SimpleMath::Vector2& b)
{
	return a.x * b.x + b.y * a.y;
}

// 2ŸŒ³‚ÌŠOÏ‚ğŒvZ‚·‚é
inline float Cross2D(const DirectX::SimpleMath::Vector2& a, const DirectX::SimpleMath::Vector2& b)
{
	return a.x * b.y - a.y * b.x;
}
