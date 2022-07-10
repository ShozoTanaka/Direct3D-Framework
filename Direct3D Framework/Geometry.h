#pragma once
#include "SimpleMath.h"
#include "Common.h"

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

// 光線と平面の交差判定を行う
inline bool IntersectRayPlane(
	const DirectX::SimpleMath::Ray& ray,
	const DirectX::SimpleMath::Plane& plane,
	DirectX::SimpleMath::Vector3* const intersection
)
{
	/*
	無限平面と交点の式　t = normal * (position - origin) / (normal * direction)
	交点 origin + direction * t

	光線: P = P0 + tV
	平面: P ・N + d = 0
	(P0 + tV)・N + d = 0
	t = -(P0・N + d) / (V・N)
	P = P0 + tV
	*/

	// 平面法線ベクトルと光線の内積を計算する
	float d = plane.Normal().Dot(ray.direction);
	// 光線と平面が平行の場合は交点は存在しない
	if (d == 0.0f)
		return false;
	// 平面の法線ベクトルと光線の原点の間の内積を計算する
	float t = (plane.D() - plane.Normal().Dot(ray.position)) / d;
	// 交差点を計算する
	*intersection = ray.position + ray.direction * t;
	// 光線と平面が交差している
	return true;
}

// 点が円の内部にあるかどうかを判定する
inline bool InsideCircle(const int& radius, const DirectX::SimpleMath::Vector2& center, const DirectX::SimpleMath::Vector2& point)
{
	// 2点間の距離を計算する
	float distance = (center.x - point.x) * (center.x - point.x) + (center.y - point.y) * (center.y - point.y);
	// 距離と半径の二乗と比較する
	if (distance <= radius * radius)
		return true;
	else
		return false;
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
