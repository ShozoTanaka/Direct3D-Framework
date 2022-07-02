#include "pch.h"
#include "Graphics.h"

std::unique_ptr<Graphics> Graphics::m_graphics = nullptr;

// DirectX Graphicsクラスのインスタンスを取得する
Graphics* const Graphics::GetInstance()
{
	if (m_graphics == nullptr)
	{
		// DirectX Graphicsクラスのインスタンスを生成する
		m_graphics.reset(new Graphics());
	}
	// DirectX Graphicsクラスのインスタンスを返す
	return m_graphics.get();
}

// コンストラクタ
Graphics::Graphics()
	:
	m_deviceResources(nullptr),			// デバイスリソース
	m_commonStates(nullptr),				// コモンステート
	m_spriteBatch(nullptr),					// スプライトバッチ
	m_spriteFont(nullptr),						// スプライトフォント
	m_basicEffect(nullptr),					// ベーシックエフェクト
	m_primitiveBatch(nullptr),				// プリミティブバッチ
	m_rasterrizerState(nullptr),				// ラスタライザーステート
	m_effectFactory(nullptr),				// エフェクトファクトリ
	m_inputLayout(nullptr),					// 入力レイアウト
	m_screenW(0),								// スクリーン幅
	m_screenH(0),									// スクリーン高
	m_view{},											// ビュー行列
	m_projection{},								// 射影行列
	m_device(nullptr),							// デバイス
	m_context(nullptr)							// デバイスコンテキスト
{
}

// デストラクタ
Graphics::~Graphics()
{
}

// 初期化する
void Graphics::Initialize(DX::DeviceResources* deviceResources, const int& width, const int& height)
{
	// スクリーンサイズを設定する
	SetScreenSize(width, height);
	// デバイスリソースを設定する
	m_deviceResources = deviceResources;
	// デバイスを取得する
	m_device = m_deviceResources->GetD3DDevice();
	// デバイスコンテキストを取得する
	m_context = m_deviceResources->GetD3DDeviceContext();

	// コモンステートを生成する
	m_commonStates = std::make_unique<DirectX::CommonStates>(m_device);
	// スプライトバッチを生成する
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(m_context);
	// ベーシックエフェクトを生成する
	m_basicEffect = std::make_unique<DirectX::BasicEffect>(m_device);
	// スプライトフォントを生成する
	m_spriteFont = std::make_unique<DirectX::SpriteFont>(m_device, L"resources\\font\\SegoeUI_18.spritefont");
	// プリミティブバッチを生成する
	m_primitiveBatch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>(m_context);
	// 入力レイアウトを生成する
	m_basicEffect->SetVertexColorEnabled(true);
	// テクスチャを無効にする
	m_basicEffect->SetTextureEnabled(false);

	void const* shaderByteCode;
	size_t byteCodeLength;

	m_basicEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
	// 入力レイアウトを生成する
	m_device->CreateInputLayout(
		DirectX::VertexPositionColor::InputElements,
		DirectX::VertexPositionColor::InputElementCount,
		shaderByteCode, byteCodeLength,
		m_inputLayout.ReleaseAndGetAddressOf()
	);
	CD3D11_RASTERIZER_DESC rasterizerStateDesc(
		D3D11_FILL_SOLID, D3D11_CULL_NONE, FALSE,
		D3D11_DEFAULT_DEPTH_BIAS, D3D11_DEFAULT_DEPTH_BIAS_CLAMP,
		D3D11_DEFAULT_SLOPE_SCALED_DEPTH_BIAS, TRUE, FALSE, FALSE, TRUE);
	// ラスタライズステートを生成する
	m_device->CreateRasterizerState(&rasterizerStateDesc, m_rasterrizerState.ReleaseAndGetAddressOf());
	// エフェクトファクトリを生成する
	m_effectFactory = std::make_unique<DirectX::EffectFactory>(m_device);
	// リソースディレクトリを設定する
	//m_fx->SetDirectory(L"resources\\cmo");
}

// 文字列を描画する
void Graphics::DrawString(const float& x, const float& y, const wchar_t* str)
{
	// 文字列を描画する
	m_spriteFont->DrawString(m_spriteBatch.get(), str, DirectX::SimpleMath::Vector2(x, y));
}

// プリミティブ描画を開始する
void Graphics::DrawPrimitiveBegin(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection)
{
	m_context->OMSetBlendState(m_commonStates->Opaque(), nullptr, 0xFFFFFFFF);
	m_context->OMSetDepthStencilState(m_commonStates->DepthNone(), 0);
	m_context->RSSetState(m_commonStates->CullNone());
	//m_context->RSSetState(m_rasterrizeState.Get());

	// ビュー行列を設定する
	m_basicEffect->SetView(view);
	// プロジェクション行列を設定する
	m_basicEffect->SetProjection(projection);
	// ワールド行列を設定する
	m_basicEffect->SetWorld(DirectX::SimpleMath::Matrix::Identity);

	// 頂点カラーを有効にする
	m_basicEffect->SetVertexColorEnabled(true);
	// テクスチャを有効にする
	m_basicEffect->SetTextureEnabled(false);
	// 入力レイアウトを設定する
	m_basicEffect->Apply(m_context);
	// 入力レイアウトを設定する
	m_context->IASetInputLayout(m_inputLayout.Get());
	// プリミティブ描画を開始する
	m_primitiveBatch->Begin();
}

// プリミティブ描画を終了する
void Graphics::DrawPrimitiveEnd()
{
	// プリミティブ描画を終了する
	m_primitiveBatch->End();
}

// 線分を描画する(XZ平面)
void Graphics::DrawLine(
	const DirectX::SimpleMath::Vector2& position,
	const DirectX::SimpleMath::Vector2& vector,
	const DirectX::FXMVECTOR& color
)
{
	// 頂点カラーを設定する
	DirectX::VertexPositionColor vertex[2] =
	{
		{ DirectX::SimpleMath::Vector3(position.x, 0.0f, position.y), color },
		{ DirectX::SimpleMath::Vector3(position.x + vector.x, 0.0f, position.y + vector.y), color },
	};
	// 線分を描画する
	m_primitiveBatch->DrawLine(vertex[0], vertex[1]);
}

// ベクトルを描画する(XZ平面)
void Graphics::DrawVector(
	const DirectX::SimpleMath::Vector2& position, 
	const DirectX::SimpleMath::Vector2& vector, 
	const DirectX::FXMVECTOR& color
)
{
	using namespace DirectX::SimpleMath;
	const float cosTheta = cosf(DirectX::XMConvertToRadians(20.0f));
	const float sinTheta = sinf(DirectX::XMConvertToRadians(20.0f));

	// 矢印のベクトルのサイズを設定する
	Vector2 arrow = -vector;
	// 正規化する
	arrow.Normalize();
	// 矢印のサイズを設定する
	arrow *= 3.0f;
	// 右矢 X: (xcosθ- ysinθ)  Y: (xsinθ+ ycosθ)
	Vector2 arrowR = Vector2(arrow.x * cosTheta - arrow.y * sinTheta, arrow.x * sinTheta + arrow.y * cosTheta);
	// 左矢 X: (xcosθ- ysinθ)  Y: (xsinθ+ ycosθ)
	Vector2 arrowL = Vector2(arrow.x * cosTheta + arrow.y * sinTheta, -arrow.x * sinTheta + arrow.y * cosTheta);
	// 線分を描画する
	DrawLine(position, vector, color);
	// 右矢を描画する
	DrawLine(position + vector, arrowR, color);
	// 左矢を描画する
	DrawLine(position + vector, arrowL, color);
}

// 円を描画する(XZ平面)
void Graphics::DrawCircle(
	const DirectX::SimpleMath::Vector2& center,
	const float& radius,
	const DirectX::FXMVECTOR& color,
	const int& split
)
{
	using namespace DirectX::SimpleMath;
	// 角度を初期化する
	float angle = 0.0f;
	// 始点を宣言する
	Vector2 position1 = center + Vector2(cosf(angle), sinf(angle)) * radius;
	for (int index = 0; index < split; index++)
	{
		// 始点を設定する
		Vector2 position0 = position1;
		// 角度を計算する
		angle += DirectX::XM_2PI / (float)split;
		// 次点を計算する
		position1 = center + Vector2(cosf(angle), sinf(angle)) * radius;
		// 円を描画する
		DrawLine(position0, position1 - position0, color);
	}
}

// モデルを描画する
void Graphics::DrawModel(const DirectX::Model* model, const DirectX::SimpleMath::Matrix& world, const bool& depthBuffer)
{
	if (depthBuffer)
	{
		// モデルを描画する
		model->Draw(m_context, *m_commonStates.get(), world, m_view, m_projection);
	}
	else
	{
		// モデルを描画する
		model->Draw(
			m_context,
			*m_commonStates.get(),
			world,
			m_view,
			m_projection,
			false,
			[&]() { 	m_context->OMSetDepthStencilState(m_commonStates->DepthNone(), 0); }
		);
	}
}

// アニメーションモデルを描画する
void Graphics::DrawModel(
	const DirectX::Model* model,
	const DX::AnimationSDKMESH* animationSDKMESH,
	const DirectX::ModelBone::TransformArray* transformArray,
	const DirectX::SimpleMath::Matrix& world
)
{
	// ボーン配列のサイズを取得する
	size_t bones = model->bones.size();
	// アニメーションにモデル、ボーン数、ボーンを適用する
	animationSDKMESH->Apply(*model, bones, transformArray->get());
	// コモンステートを取得する
	DirectX::CommonStates* commonState = Graphics::GetInstance()->GetCommonStates();
	// アニメーションを描画する
	model->DrawSkinned(m_context, *commonState, bones, transformArray->get(), world, m_view, m_projection);
}

