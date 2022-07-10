#pragma once
#ifndef GRAPHICS_DEFINED
#define GRAPHICS_DEFINED
#include "DeviceResources.h"
#include "Animation.h"

// DirectX Graphicsクラス
class Graphics final
{
public:
	// DirectX Graphicsクラスのインスタンスを取得する
	static Graphics* const GetInstance();

	// スクリーンサイズを取得する
	void GetScreenSize(int& width, int& height) const { 	width = m_screenW; height = m_screenH; }
	// スクリーンサイズを設定する
	void SetScreenSize(const int& width, const int& height) { m_screenW = width; m_screenH = height; }
	// デバイスリソースを取得する
	DX::DeviceResources* GetDeviceResources() const { return m_deviceResources; }
	// PrimitiveBatchクラスのインスタンスを取得する
	DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* GetPrimitiveBatch() const { return m_primitiveBatch.get(); }
	// InputLayoutクラスのインスタンスを取得する
	ID3D11InputLayout* GetInputLayout() const { return m_inputLayout.Get(); }
	// BasicEffectクラスのインスタンス取得する
	DirectX::BasicEffect* GetBasicEffect() { return m_basicEffect.get(); }
	// CommonStatesクラスのインスタンスを取得する
	DirectX::CommonStates* GetCommonStates() const { return m_commonStates.get(); }
	// SpriteBatchクラスのインスタンスを取得する
	DirectX::SpriteBatch* GetSpriteBatch() { return m_spriteBatch.get(); }
	// SpriteFontクラスのインスタンスを取得する
	DirectX::SpriteFont* GetFont() { return m_spriteFont.get(); }
	// EffectFactoryクラスのインスタンスを取得する
	DirectX::EffectFactory* GetFX() const { return m_effectFactory.get(); }
	// ビュー行列を設定する
	void SetViewMatrix(const DirectX::SimpleMath::Matrix& view) { m_view = view; }
	// ビュー行列を取得する
	const DirectX::SimpleMath::Matrix& GetViewMatrix() { return m_view; };
	// 射影行列を設定する
	void SetProjectionMatrix(const DirectX::SimpleMath::Matrix& projection) { m_projection = projection; }
	// 射影行列を取得する
	const DirectX::SimpleMath::Matrix& GetProjectionMatrix() { return m_projection; };

public:
	// デストラクタ
	~Graphics();
	// 初期化する
	void Initialize(DX::DeviceResources* deviceResources, const int& width, const int& height);
	// 文字列を描画する
	void DrawString(const float& x, const float& y, const wchar_t* str);
	// プリミティブ描画を開始する
	void DrawPrimitiveBegin(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection);
	// プリミティブ描画を終了する
	void DrawPrimitiveEnd();
	// 線分を描画する
	void DrawLine(const DirectX::SimpleMath::Vector2& position, const DirectX::SimpleMath::Vector2& vector, const DirectX::FXMVECTOR& m_color = DirectX::Colors::White);
	// ベクトルを描画する
	void DrawVector(const DirectX::SimpleMath::Vector2& pos, const DirectX::SimpleMath::Vector2& vector, const DirectX::FXMVECTOR& color = DirectX::Colors::White);
	// 円を描画する
	void DrawCircle(const DirectX::SimpleMath::Vector2& center, const float& radius, const DirectX::FXMVECTOR& m_color = DirectX::Colors::White, const int& split = 32);
	// モデルを描画する
	void DrawModel(const DirectX::Model* model, const DirectX::SimpleMath::Matrix& world, const bool& depthBuffer = true);
	// アニメーションモデルを描画する
	void DrawModel(
		const DirectX::Model* model,
		const DX::AnimationSDKMESH* animationSDKMESH,
		const DirectX::ModelBone::TransformArray* transformArray,
		const DirectX::SimpleMath::Matrix& world
	);

private:
	// コンストラクタ
	Graphics();

	// 代入は許容しない
	void operator=(const Graphics& object) = delete;
	// コピーコンストラクタは許容しない
	Graphics(const Graphics& object) = delete;

private:
	// Graphicsクラスのインスタンスへのポインタ
	static std::unique_ptr<Graphics> m_graphics;
	// デバイスリソースへのポインタ
	DX::DeviceResources* m_deviceResources;
	// コモンステート
	std::unique_ptr<DirectX::CommonStates> m_commonStates;
	// スプライトバッチ
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	// スプライトフォント
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;
	// ベーシックエフェクト
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	// プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;
	// エフェクトファクトリー
	std::unique_ptr<DirectX::EffectFactory> m_effectFactory;
	// ラスタライザーステート
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_rasterrizerState;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	// スクリーンサイズ
	int m_screenW;
	int m_screenH;
	// ビュー行列
	DirectX::SimpleMath::Matrix m_view;
	// 射影行列
	DirectX::SimpleMath::Matrix m_projection;
	// デバイス
	ID3D11Device* m_device;
	// デバイスコンテキスト
	ID3D11DeviceContext* m_context;
};

#endif		// GRAPHICS_DEFINED

