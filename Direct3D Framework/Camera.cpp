#include "pch.h"
#include "Camera.h"

// コンストラクタ
Camera::Camera()
	:
	m_eyePosition(DirectX::SimpleMath::Vector3(0.0f, 300.0f, 0.0f)),		// 視点の位置
	m_targetPosition(DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f))		// 注視点の位置
{
}

// デストラクタ
Camera::~Camera()
{
}

// ビュー行列を更新する
void Camera::Update()
{
	// カメラのUPベクトルを計算する
	DirectX::SimpleMath::Vector3 toEye = GetEyePosition() - GetTargetPosition();
	// 正規化する
	toEye.Normalize();
	// UPベクトルを計算する
	DirectX::SimpleMath::Vector3 up = toEye.Cross(DirectX::SimpleMath::Vector3(1.0f, 0.0f, 0.0f));
	// ビュー行列を生成する
	DirectX::SimpleMath::Matrix view = DirectX::SimpleMath::Matrix::CreateLookAt(
		GetEyePosition(),
		GetTargetPosition(),
		up
	);
	// ビュー行列を設定する
	Graphics::GetInstance()->SetViewMatrix(view);
}
