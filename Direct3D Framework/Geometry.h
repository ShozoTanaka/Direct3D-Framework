#pragma once
#include "SimpleMath.h"

// 2�����̓��ς��v�Z����
inline float Dot2D(const DirectX::SimpleMath::Vector2& a, const DirectX::SimpleMath::Vector2& b)
{
	return a.x * b.x + b.y * a.y;
}

// 2�����̊O�ς��v�Z����
inline float Cross2D(const DirectX::SimpleMath::Vector2& a, const DirectX::SimpleMath::Vector2& b)
{
	return a.x * b.y - a.y * b.x;
}

// �~�Ɛ����̌���������s��
inline bool IntersectCircleLine(
	const DirectX::SimpleMath::Vector2& center,					// ���S�_
	const float& radius,																// ���a
	const DirectX::SimpleMath::Vector2& start,						// �����̊J�n
	const DirectX::SimpleMath::Vector2& end							// �����̏I��
)
{
	// �����̊J�n�n�_����~�̒��S�ւ̃x�N�g��
	DirectX::SimpleMath::Vector2 startToCenter = center - start;
	// �����̏I���n�_����~�̒��S�ւ̃x�N�g��
	DirectX::SimpleMath::Vector2 endToCenter = center - end;
	// �����̊J�n�ʒu����I���ʒu�ւ̃x�N�g��
	DirectX::SimpleMath::Vector2 startToEnd = end - start;
	// �~�̒��S��������܂ł̋������v�Z����
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
