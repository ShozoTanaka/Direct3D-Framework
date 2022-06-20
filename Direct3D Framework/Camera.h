#pragma once
#ifndef CAMERA_DEFINED
#define CAMERA_DEFINED

class Camera
{
public:
	// 視点の位置を取得する
	const DirectX::SimpleMath::Vector3& GetEyePosition() const
	{
		return m_eyePosition;
	}
	// 視点の位置を設定する
	void SetEyePosition(const DirectX::SimpleMath::Vector3& eyePosition)
	{
		m_eyePosition = eyePosition;
	}
	// 注視点の位置を取得する
	DirectX::SimpleMath::Vector3& GetTargetPosition()
	{
		return m_targetPosition;
	}

	// コンストラクタ
	Camera();
	// デストラクタ
	~Camera();
	// 更新する
	void Update();

private:
	// 視点の位置
	DirectX::SimpleMath::Vector3 m_eyePosition;
	// 注視点の位置
	DirectX::SimpleMath::Vector3 m_targetPosition;
};

#endif		// CAMERA_DEFINED
