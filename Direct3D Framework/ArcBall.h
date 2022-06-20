//--------------------------------------------------------------------------------------
// File: ArcBall.h
// Ken Shoemake, "Arcball Rotation Control", Graphics Gems IV, pg 176 - 192  
// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.
//--------------------------------------------------------------------------------------

#pragma once
#ifndef ARC_BALL_DEFINED
#define ARC_BALL_DEFINED

/// <summary>
/// ArcBallクラス
/// </summary>
class ArcBall
{
public:
    /// <summary>
    /// コンストラクタ
    /// </summary>
    ArcBall()
        :
        m_width(1280.f),
        m_height(720.f),
        m_radius(1.f),
        m_drag(false) 
    {
        Reset();  
    }

    /// <summary>
    /// リセットする
    /// </summary>
    void Reset()
    {
        m_qdown = m_qnow = DirectX::SimpleMath::Quaternion::Identity;
    }
    /// <summary>
    /// 開始時
    /// </summary>
    /// <param name="x">X座標</param>
    /// <param name="y">Y座標</param>
    void OnBegin( int x, int y )
    {
        m_drag = true;
        m_qdown = m_qnow;
        m_downPoint = ScreenToVector(float(x), float(y));
    }

    /// <summary>
    /// 移動時
    /// </summary>
    /// <param name="x">X座標</param>
    /// <param name="y">Y座標</param>
    void OnMove(int x, int y)
    {
        using namespace DirectX;
        if (m_drag)
        {
            XMVECTOR current = ScreenToVector(float(x), float(y));

            m_qnow = XMQuaternionMultiply(m_qdown, QuatFromBallPoints(m_downPoint, current));
            // 正規化する
            m_qnow.Normalize();
        }
    }

    /// <summary>
    /// 終了時
    /// </summary>
    void OnEnd()
    {
        m_drag = false;
    }

    /// <summary>
    /// ウィンドウを設定する
    /// </summary>
    /// <param name="width">幅</param>
    /// <param name="height">高さ</param>
    void SetWindow(const int& width, const int& height)
    {
        m_width = float(width);
        m_height = float(height);
    }

    /// <summary>
    /// 半径を設定する
    /// </summary>
    /// <param name="radius">半径</param>
    void SetRadius(const float& radius)
    {
        m_radius = radius;
    }

    /// <summary>
    /// クォータニオンを取得する
    /// </summary>
    /// <returns>現在のクォータニオン</returns>
    DirectX::SimpleMath::Quaternion GetQuaternion() const 
    { 
        return m_qnow; 
    }

    /// <summary>
    /// ドラッグしているかどうかを調べる
    /// </summary>
    /// <returns></returns>
    bool IsDragging() const 
    { 
        return m_drag; 
    }

private:
    DirectX::XMVECTOR ScreenToVector(const float& screenx, const float& screeny)
    {
        float x = -( screenx - m_width / 2.f ) / ( m_radius * m_width / 2.f );
        float y = ( screeny - m_height / 2.f ) / ( m_radius * m_height / 2.f );

        float z = 0.0f;
        float magitude = x * x + y * y;

        if( magitude > 1.0f )
        {
            float scale = 1.0f / sqrtf( magitude );
            x *= scale;
            y *= scale;
        }
        else
            z = sqrtf( 1.0f - magitude );

        return DirectX::XMVectorSet( x, y, z, 0 );
    }

    static DirectX::XMVECTOR QuatFromBallPoints(const DirectX::FXMVECTOR& vFrom, const DirectX::FXMVECTOR& vTo )
    {
        using namespace DirectX;
        XMVECTOR dot = XMVector3Dot( vFrom, vTo );
        XMVECTOR vPart = XMVector3Cross( vFrom, vTo );
        return XMVectorSelect( dot, vPart, g_XMSelect1110 );
    }

private:
    float  m_width;
    float  m_height;
    float  m_radius;
    DirectX::SimpleMath::Quaternion m_qdown;
    DirectX::SimpleMath::Quaternion m_qnow;
    DirectX::SimpleMath::Vector3 m_downPoint;
    bool  m_drag;
};

#endif      // ARC_BALL_DEFINED
