#pragma once
#include "SimpleMath.h"

// 2次元の内積を計算する
inline float Dot2D(const DirectX::SimpleMath::Vector2& a, const DirectX::SimpleMath::Vector2& b)
{
	return a.x * b.x + b.y * a.y;
}

// 2次元の外積を計算する
inline float Cross2D(const DirectX::SimpleMath::Vector2& a, const DirectX::SimpleMath::Vector2& b)
{
	return a.x * b.y - a.y * b.x;
}

// 円と線分の交差判定を行う
inline bool IntersectCircleLine(
	const DirectX::SimpleMath::Vector2& center,					// 中心点
	const float& radius,																// 半径
	const DirectX::SimpleMath::Vector2& start,						// 線分の開始
	const DirectX::SimpleMath::Vector2& end							// 線分の終了
)
{
	// 線分の開始地点から円の中心へのベクトル
	DirectX::SimpleMath::Vector2 startToCenter = center - start;
	// 線分の終了地点から円の中心へのベクトル
	DirectX::SimpleMath::Vector2 endToCenter = center - end;
	// 線分の開始位置から終了位置へのベクトル
	DirectX::SimpleMath::Vector2 startToEnd = end - start;
	// 円の中心から線分までの距離を計算する
	float length = Cross2D(startToCenter, startToEnd) / startToEnd.Length();

	if (abs(length) <= radius)
	{
		if (Dot2D(startToCenter, startToEnd) * Dot2D(endToCenter, startToEnd) <= 0)
			return true;
		else if (radius > startToCenter.Length() || radius > endToCenter.Length())
			return true;
	}
	return false;
}
