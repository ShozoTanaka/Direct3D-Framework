#include "pch.h"
#include "SpriteString3D.h"
#include "Graphics.h"

/// <summary>フォントサイズ</summary>
const float SpriteString3D::DEFAULT_FONT_SIZE = 0.1f;

// コンストラクタ
SpriteString3D::SpriteString3D()
	:
	m_inputLayout(nullptr),		// 入力レイアウト
	m_strings{},							// 表示文字列の配列
	m_spriteBatch(nullptr),		// スプライトバッチ
	m_basicEffect(nullptr),		// スプライトバッチ
	m_spriteFont(nullptr)			// スプライトフォント
{
	// DirectX Graphicsを取得する
	m_directXGraphics = Graphics::GetInstance();
	// デバイスを取得する
	m_device = m_directXGraphics->GetDeviceResources()->GetD3DDevice();
	// デバイスコンテキストを取得する
	m_context = m_directXGraphics->GetDeviceResources()->GetD3DDeviceContext();
	// エフェクトを取得する
	m_basicEffect = m_directXGraphics->GetBasicEffect();
	// テクスチャを有効にする
	m_basicEffect->SetTextureEnabled(true);
	// 頂点カラーを有効にする
	m_basicEffect->SetVertexColorEnabled(true);

	void const* shaderByteCode;
	size_t byteCodeLength;
	m_basicEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

	// 入力レイアウトを作成する
	m_device->CreateInputLayout(
		DirectX::VertexPositionColorTexture::InputElements,
		DirectX::VertexPositionColorTexture::InputElementCount,
		shaderByteCode, byteCodeLength,
		m_inputLayout.GetAddressOf()
	);
}

// デストラクタ
SpriteString3D::~SpriteString3D()
{
}

// 描画する文字列を登録する
void SpriteString3D::AddString(
	const wchar_t* string,
	const DirectX::SimpleMath::Vector3& position,
	const DirectX::FXMVECTOR& color,
	const float& size
)
{
	// 内部文字列を宣言する
	InnerString innerString;
	// 文字列
	innerString.m_string = std::wstring(string);
	// 位置
	innerString.m_position = position;
	// 色
	innerString.m_color = color;
	// サイズ
	innerString.m_size = size;
	// 文字列配列に文字列を追加する
	m_strings.push_back(innerString);
}

// 文字列配列のインデックス番目を削除する
void SpriteString3D::RemoveString(const int& index)
{
	// 文字列リストから文字列を削除する
	m_strings.erase(m_strings.begin() + index);
}

// すべての文字列配列を削除する
void SpriteString3D::RemoveAllString()
{
	// 配列をクリアする
	m_strings.clear();
}


// 描画する
void SpriteString3D::Render()
{
	// スプライトバッチを取得する
	m_spriteBatch = m_directXGraphics->GetSpriteBatch();
	// フォントを取得する
	m_spriteFont = m_directXGraphics->GetFont();
	// ベイシックエフェクトを取得する
	m_basicEffect = m_directXGraphics->GetBasicEffect();
	// テクスチャを有効にする
	m_basicEffect->SetTextureEnabled(true);
	// 頂点カラーを有効にする
	m_basicEffect->SetVertexColorEnabled(true);
	// ビュー行列を設定する
	m_basicEffect->SetView(m_directXGraphics->GetViewMatrix());
	// プロジェクション行列を設定する
	m_basicEffect->SetProjection(m_directXGraphics->GetProjectionMatrix());

	// ビュー行列の逆行列を作成する
	DirectX::SimpleMath::Matrix invertView = m_directXGraphics->GetViewMatrix().Invert();
	invertView._41 = 0.0f;
	invertView._42 = 0.0f;
	invertView._43 = 0.0f;

	// スクリーン座標はY軸の上下が反転する
	DirectX::SimpleMath::Matrix scaleY;
	scaleY._22 = -1.0f;

	for (size_t index = 0; index < m_strings.size(); index++)
	{
		// スプライトバッチを開始する
		m_spriteBatch->Begin(
			DirectX::SpriteSortMode_Deferred,
			nullptr,
			nullptr,
			m_directXGraphics->GetCommonStates()->DepthNone(),
			m_directXGraphics->GetCommonStates()->CullNone(),
			[=]
			{
				// 移動行列を計算する
				DirectX::SimpleMath::Matrix translation = DirectX::SimpleMath::Matrix::CreateTranslation(m_strings[index].m_position);
				// ワールド行列を計算する
				DirectX::SimpleMath::Matrix world = scaleY * invertView * translation;
				// ワールド行列を設定する
				m_basicEffect->SetWorld(world);
				// エフェクトを適用する
				m_basicEffect->Apply(m_context);
				// 入力レイアウトを設定する
				m_context->IASetInputLayout(m_inputLayout.Get());
			}
		);
		// テキストの描画位置を設定する
		DirectX::XMVECTOR textOrigin = (DirectX::SimpleMath::Vector2)m_spriteFont->MeasureString(m_strings[index].m_string.c_str()) / 2.0f;
		// 文字列を描画する
		m_spriteFont->DrawString(
			m_spriteBatch,
			m_strings[index].m_string.c_str(),
			DirectX::SimpleMath::Vector2::Zero,
			m_strings[index].m_color,
			0.0f,
			textOrigin,
			m_strings[index].m_size
		);
		// スプライトバッチを終了する
		m_spriteBatch->End();
	}
}

