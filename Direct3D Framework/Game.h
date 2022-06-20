//
// Game.h
//

#pragma once
#ifndef GAME_DEFINED
#define GAME_DEFINED

#include "DeviceResources.h"
#include "StepTimer.h"
#include "Camera.h"
#include "GraphScene.h"

class GraphScene;

class Game final : public DX::IDeviceNotify
{
    HWND GetWindowHandle() { return m_hWnd; }

public:
    // キーボードを取得する
    DirectX::Keyboard* GetKeyboard() const { return m_keyboard.get(); }
    // キーボードステートトラッカを取得する
    DirectX::Keyboard::KeyboardStateTracker& GetKeyboardTracker() const { return *m_keyboardTracker.get(); }
    // マウスを取得する
    DirectX::Mouse* GetMouse() const { return m_mouse.get(); }
    // カメラを取得する
    Camera* GetCamera() const { return m_camera.get(); }
public:
    // コンストラクタ
    Game() noexcept(false);
    // デストラクタ
    ~Game() = default;
    Game(Game&&) = default;
    Game& operator= (Game&&) = default;
    Game(Game const&) = delete;
    Game& operator= (Game const&) = delete;

    // 初期化する
    void Initialize(HWND hWnd, int width, int height);

    // ゲームループ
    void Tick();

    // IDeviceNotify
    void OnDeviceLost() override;
    void OnDeviceRestored() override;

    // メッセージハンドラ
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowMoved();
    void OnWindowSizeChanged(int width, int height);

    // プロパティ
    void GetDefaultSize( int& width, int& height ) const noexcept;

private:
    // 更新する
    void Update(DX::StepTimer const& timer);
    // 描画する
    void Render();
    // バックバッファをクリアする
    void Clear();

    void CreateDeviceDependentResources();
    void CreateWindowSizeDependentResources();

private:
    // ウィンドウハンドル
    HWND m_hWnd;
    // デバイスリソース
    std::unique_ptr<DX::DeviceResources> m_deviceResources;
    // タイマー
    DX::StepTimer m_timer;
    // マウス
    std::unique_ptr<DirectX::Mouse> m_mouse;
    // キーボード
    std::unique_ptr<DirectX::Keyboard> m_keyboard;
    // キーボードトラッカー
    std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker> m_keyboardTracker;
    // カメラ
    std::unique_ptr<Camera> m_camera;
    // カメラの位置
    DirectX::SimpleMath::Vector3 m_eyePosition;

    // GraphScene
    std::unique_ptr<GraphScene> m_graphScene;
};

#endif      // GAME_DEFINED