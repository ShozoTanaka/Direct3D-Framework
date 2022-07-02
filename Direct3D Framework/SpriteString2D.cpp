#include "pch.h"
#include "SpriteString2D.h"
#include "Graphics.h"

// フォントサイズ
const float SpriteString2D::DEFAULT_FONT_SIZE = 1.0f;

// コンストラクタ
SpriteString2D::SpriteString2D()
	:
	m_strings{}
{
}

// デストラクタ
SpriteString2D::~SpriteString2D()
{
	m_strings.clear();
}

// 描画する文字列を追加する
void SpriteString2D::AddString(
	const wchar_t* string,
	const DirectX::SimpleMath::Vector2& position,
	const DirectX::FXMVECTOR& color,
	const float& size
)
{
	// 内部文字列を宣言する
	InnerString innerString;
	// 文字列を代入する
	innerString.m_string = std::wstring(string);
	// 位置を設定する
	innerString.m_position = position;
	// 色を設定する
	innerString.m_color = color;
	// サイズを設定する
	innerString.m_size = size;
	// 文字列配列に文字列を追加する
	m_strings.push_back(innerString);
}

// 文字列を削除する
void SpriteString2D::RemoveString(const int& index)
{
	// 文字列リストから文字列を削除する
	m_strings.erase(m_strings.begin() + index);
}

// すべての文字列配列を削除する
void SpriteString2D::RemoveAllString()
{
	// 配列をクリアする
	m_strings.clear();
}

// 描画する
void SpriteString2D::Render()
{
	// スプライトバッチを取得する
	DirectX::SpriteBatch* spriteBatch = Graphics::GetInstance()->GetSpriteBatch();
	// フォントを取得する
	DirectX::SpriteFont* font = Graphics::GetInstance()->GetFont();
	// スプライトバッチを開始する
	spriteBatch->Begin();
	// 追加された文字列を順番に取得する
	for (size_t index = 0; index < m_strings.size(); index++)
	{
		// 文字列を描画する
		font->DrawString(
			spriteBatch,
			m_strings[index].m_string.c_str(),
			m_strings[index].m_position,
			m_strings[index].m_color,
			0.0f,
			DirectX::SimpleMath::Vector2(0.0f, 0.0f),
			m_strings[index].m_size
		);
	}
	// スプライトバッチを終了する
	spriteBatch->End();
}
