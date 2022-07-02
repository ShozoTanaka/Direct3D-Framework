#pragma once
#ifndef SPRITE_STRING_3D_DEFINED
#define SPRITE_STRING_3D_DEFINED
#include <vector>
#include <string>
#include "Graphics.h"

class SpriteString3D
{
private:

	// InnerStringクラス
	struct InnerString
	{
		// 位置
		DirectX::SimpleMath::Vector3 m_position;
		// 文字列
		std::wstring m_string;
		// 色
		DirectX::SimpleMath::Color m_color;
		// サイズ
		float m_size;
	};

public:
	// コンストラクタ
	SpriteString3D();
	// デストラクタ
	~SpriteString3D();
	// 代入演算子削除
	void operator=(const SpriteString3D& object) = delete;
	// コピーコンストラクタ削除
	SpriteString3D(const SpriteString3D& object) = delete;

	// 描画する文字列を追加する
	void AddString	(
		const wchar_t* string,
		const DirectX::SimpleMath::Vector3& position,
		const DirectX::FXMVECTOR& color = DirectX::Colors::White,
		const float& size = DEFAULT_FONT_SIZE
	);
	// 文字列を削除する
	void RemoveString(const int& index);
    // すべての文字列配列を削除する
	void RemoveAllString();
	// 描画する
	void Render();

private:
	// フォントサイズ
	static const float DEFAULT_FONT_SIZE;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	// 表示文字列配列
	std::vector<InnerString> m_strings;

	// DirectX Graphics
	Graphics* m_directXGraphics;
	// デバイス
	ID3D11Device* m_device;
	// デバイスコンテキスト
	ID3D11DeviceContext* m_context;
	// スプライトバッチ
	DirectX::SpriteBatch* m_spriteBatch;
	// ベイシックエフェクト
	DirectX::BasicEffect* m_basicEffect;
	// スプライトフォント
	DirectX::SpriteFont* m_spriteFont;
};

#endif	// SPRITE_STRING_3D_DEFINED


