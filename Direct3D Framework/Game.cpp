//
// Game.cpp
//

#include "pch.h"
#include "Game.h"

extern void ExitGame() noexcept;

// コンストラクタ
Game::Game() noexcept(false)
    :
    m_hWnd(0),                                                                     // ウィンドウハンドル
    m_deviceResources(nullptr),                                          // デバイスリソース
    m_timer{},                                                                       // タイマー
    m_mouse(nullptr),                                                          // マウス
    m_keyboard(nullptr),                                                      // キーボード
    m_keyboardTracker(nullptr),                                          // キーボードトラッカー
    m_camera(nullptr),                                                        // カメラ
    m_eyePosition(DirectX::SimpleMath::Vector3::Zero),  // カメラ位置
    m_graphScene{}                                                           // GraphSceneクラス
{
    // デバイスリソースを生成する
    m_deviceResources = std::make_unique<DX::DeviceResources>();
    // デバイス通知を登録する
    m_deviceResources->RegisterDeviceNotify(this);
}

// 初期化する
void Game::Initialize(HWND hWnd, int width, int height)
{
    // ウィンドウハンドルを設定する
    m_hWnd = hWnd;
    // ウィンドウ情報を設定する
    m_deviceResources->SetWindow(hWnd, width, height);
    // デバイスリソースを生成する
    m_deviceResources->CreateDeviceResources();
    // デバイスに依存したリソースを生成する
    CreateDeviceDependentResources();
    // ウィンドウサイズに依存したリソースを生成する
    m_deviceResources->CreateWindowSizeDependentResources();
    // ウィンドウサイズに応じたリソースを生成する
    CreateWindowSizeDependentResources();

    // TODO: Change the timer settings if you want something other than the default variable timestep mode.
    // e.g. for 60 FPS fixed timestep update logic, call:
    /*
    m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 60);
    */
    // マウスのインスタンスを生成する
    m_mouse = std::make_unique<DirectX::Mouse>();
    // ウィンドウサイズを設定する
    m_mouse->SetWindow(hWnd);
    // キーボードのインスタンスを生成する
    m_keyboard = std::make_unique<DirectX::Keyboard>();
    // キーボードトラッカーのインスタンスを生成する
    m_keyboardTracker = std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();
    // 画面サイズ
    int screenWidth, screenHeight;
    // 画面サイズを取得する
    GetDefaultSize(screenWidth, screenHeight);
    // 画面サイズを設定する
    Graphics::GetInstance()->SetScreenSize(screenWidth, screenHeight);
    // カメラを生成する
    m_camera = std::make_unique<Camera>();
    // 射影行列を生成する
    DirectX::SimpleMath::Matrix projection =
        DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(
            DirectX::XMConvertToRadians(40.0f),
            screenWidth / (float)screenHeight,
            0.1f,
            1000.0f
        );
    // 射影行列を設定する
    Graphics::GetInstance()->SetProjectionMatrix(projection);

    // GraphSceneクラスのインスタンスを生成する
    m_graphScene = std::make_unique<GraphScene>(this);
    // GraphSceneクラスのインスタンスを初期化する
    m_graphScene->Initialize();
}

#pragma region Frame Update
// ゲームループを実行する
void Game::Tick()
{
    m_timer.Tick([&]()
    {
        // 更新する
        Update(m_timer);
    });
    // 描画する
    Render();
}

// 更新する
void Game::Update(DX::StepTimer const& timer)
{
    // GraphSceneクラスのインスタンスを更新する
    m_graphScene->Update(timer);
    // Cameraクラスのインスタンスを更新する
    m_camera->Update();
}
#pragma endregion

#pragma region Frame Render
// シーンを描画する
void Game::Render()
{
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }
    // バックバッファをクリアする
    Clear();
    // パフォーマンス開始イベント
    m_deviceResources->PIXBeginEvent(L"Render");
    // GraphSceneを描画する
    m_graphScene->Render();
    // パフォーマンス終了イベント
    m_deviceResources->PIXEndEvent();
    // 新規フレームを描画する
    m_deviceResources->Present();
}

// バックバッファをクリアする
void Game::Clear()
{
    m_deviceResources->PIXBeginEvent(L"Clear");

    // ビューをクリアする
    auto context = m_deviceResources->GetD3DDeviceContext();
    auto renderTarget = m_deviceResources->GetRenderTargetView();
    auto depthStencil = m_deviceResources->GetDepthStencilView();

    context->ClearRenderTargetView(renderTarget, DirectX::Colors::CornflowerBlue);
    context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    context->OMSetRenderTargets(1, &renderTarget, depthStencil);

    // ビューポートを設定する
    auto viewport = m_deviceResources->GetScreenViewport();
    context->RSSetViewports(1, &viewport);

    m_deviceResources->PIXEndEvent();
}
#pragma endregion

#pragma region Message Handlers
// メッセージハンドラ
void Game::OnActivated()
{
    // TODO: Game is becoming active window.
}

void Game::OnDeactivated()
{
    // TODO: Game is becoming background window.
}

void Game::OnSuspending()
{
    // TODO: Game is being power-suspended (or minimized).
}

void Game::OnResuming()
{
    m_timer.ResetElapsedTime();

    // TODO: Game is being power-resumed (or returning from minimize).
}

void Game::OnWindowMoved()
{
    auto r = m_deviceResources->GetOutputSize();
    m_deviceResources->WindowSizeChanged(r.right, r.bottom);
}

void Game::OnWindowSizeChanged(int width, int height)
{
    if (!m_deviceResources->WindowSizeChanged(width, height))
        return;

    CreateWindowSizeDependentResources();

    // TODO: Game window is being resized.
}

// Properties
void Game::GetDefaultSize(int& width, int& height) const noexcept
{
    // TODO: Change to desired default window size (note minimum size is 320x200).
    width = 1280;
    height = 720;
}
#pragma endregion

#pragma region Direct3D Resources
// These are the resources that depend on the device.
void Game::CreateDeviceDependentResources()
{
    // TODO: デバイスに依存したオブジェクトを初期化する(ウィンドウサイズから独立した)
    int width, height;
    // ウィンドウの既定サイズを取得する
    GetDefaultSize(width, height);
    // DirectXを初期化する
    Graphics::GetInstance()->Initialize(m_deviceResources.get(), width, height);
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateWindowSizeDependentResources()
{
    // 出力サイズを取得する
    auto m_size = m_deviceResources->GetOutputSize();
    // アスペクト率を計算する
    float aspectRatio = float(m_size.right) / float(m_size.bottom);
    // 視野角を設定する
    float fovAngleY = 45.0f * DirectX::XM_PI / 180.0f;

    // アプリが縦向きまたはスナップビューのときに変更する
    if (aspectRatio < 1.0f)
    {
        fovAngleY *= 2.0f;
    }
    // 射影行列を生成する
    DirectX::SimpleMath::Matrix projection = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView
    (
        fovAngleY,
        aspectRatio,
        0.1f,
        1000.0f
    );
    // 射影行列を設定する
    Graphics::GetInstance()->SetProjectionMatrix(projection);

    if (m_graphScene)
    {
        // アークボールのウィンドウサイズを設定する
        m_graphScene->SetWindow(m_size.right, m_size.bottom);
    }
}

void Game::OnDeviceLost()
{
    // TODO: Add Direct3D resource cleanup here.
}

void Game::OnDeviceRestored()
{
    CreateDeviceDependentResources();

    CreateWindowSizeDependentResources();
}
#pragma endregion
