#pragma once
#ifndef SPRITE_STRING2D_DEFINED
#define SPRITE_STRING2D_DEFINED
#include <vector>
#include <string>

class SpriteString2D
{
public:
	// フォントサイズ
	static const float DEFAULT_FONT_SIZE;

private:
	// InnerStringクラス
	struct InnerString
	{
		// 位置
		DirectX::SimpleMath::Vector2 m_position;
		// 文字列
		std::wstring m_string;
		// 色
		DirectX::SimpleMath::Color m_color;
		// サイズ
		float m_size;
	};
public:
	// コンストラクタ
	SpriteString2D();
	// デストラクタ
	~SpriteString2D();
	// 代入演算子削除
	void operator=(const SpriteString2D& object) = delete;
	// コピーコンストラクタ削除
	SpriteString2D(const SpriteString2D& object) = delete;
	// 描画する文字列を追加する
	void AddString(
		const wchar_t* m_string,
		const DirectX::SimpleMath::Vector2& m_position,
		const DirectX::FXMVECTOR& m_color = DirectX::Colors::White,
		const float& m_size = DEFAULT_FONT_SIZE
	);
	// 文字列を削除する
	void RemoveString(const int& index);
	// すべての文字列配列を削除する
	void RemoveAllString();
	// 描画する
	void Render();

private:
	// 表示文字列の配列
	std::vector<InnerString> m_strings;
};

#endif	// SPRITE_STRING2D_DEFINED