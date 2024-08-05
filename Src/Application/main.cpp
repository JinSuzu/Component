#include "main.h"

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_  HINSTANCE, _In_ LPSTR, _In_ int)
{
	// メモリリークを知らせる
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// COM初期化
	if (FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED)))
	{
		CoUninitialize();

		return 0;
	}

	// mbstowcs_s関数で日本語対応にするために呼ぶ
	setlocale(LC_ALL, "japanese");

	//===================================================================
	// 実行
	//===================================================================
	Application::Instance().Execute();

	// COM解放
	CoUninitialize();

	return 0;
}

void Application::Execute()
{
	int winodwSize[2] = { 0,0 };
	ConfigManger config;
	if (config.Load("WindowSize"))
	{
		m_windowSize[0] = config["x"];
		m_windowSize[1] = config["y"];
	}
	else
	{
		config["x"] = m_windowSize[0] = 1280;
		config["y"] = m_windowSize[1] = 720;
	}
	//===================================================================
	// 初期設定(ウィンドウ作成、Direct3D初期化など)
	//===================================================================
	if (Application::Instance().Init(m_windowSize[0], m_windowSize[1]) == false) {
		return;
	}

	//===================================================================
	// ゲームループ
	//===================================================================

	// 時間
	m_fpsController.Init();
	// ループ
	Math::Vector2 postMousePos;
	while (1)
	{
		// 処理開始時間Get
		m_fpsController.UpdateStartTime();

		std::string titlebar = "Kernel Engine	fps : " + std::to_string(m_fpsController.m_nowfps);
		SetWindowTextA(GetWindowHandle(), titlebar.c_str());

		m_mouseDelta = GetMouse() - postMousePos;
		postMousePos = GetMouse();

		// ゲーム終了指定があるときはループ終了
		if (m_endFlag)
		{
			break;
		}

		//=========================================
		// ウィンドウ関係の処理
		//=========================================

		// ウィンドウのメッセージを処理する
		m_window.ProcessMessage();

		// ウィンドウが破棄されてるならループ終了
		if (m_window.IsCreated() == false)
		{
			break;
		}

		//=========================================
		// アプリケーション更新処理
		//=========================================
		KernelEngine::Update();

		//=========================================
		// アプリケーション描画処理
		//=========================================
		KernelEngine::Draw();

		//=========================================
		// フレームレート制御
		//=========================================

		m_fpsController.Update();
	}

	//===================================================================
	// アプリケーション解放
	//===================================================================
	Release();
}

Math::Vector2 Application::GetMouse()
{
	POINT mouse;
	//ディスプレイの左上が(0,0)
	GetCursorPos(&mouse);

	ScreenToClient(m_window.GetWndHandle(), &mouse);

	const int* size = GetWindowSize();
	mouse.x -= (long)(*size / 2.0f);
	mouse.y -= (long)(*(++size) / 2.0f);

	return { (float)mouse.x,-(float)mouse.y };
}

bool Application::Init(int w, int h)
{
	//===================================================================
	// ウィンドウ作成
	//===================================================================
	if (!m_window.IsCreated() && m_window.Create(w, h, "Kernel Engine", "Window") == false) {
		MessageBoxA(nullptr, "ウィンドウ作成に失敗", "エラー", MB_OK);
		return false;
	}

	//===================================================================
	// フルスクリーン確認
	//===================================================================
	bool bFullScreen = false;
	/*
	if (MessageBoxA(m_window.GetWndHandle(), "フルスクリーンにしますか？", "確認", MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2) == IDYES) {
		bFullScreen = true;
	}
	*/

	//===================================================================
	// Direct3D初期化
	//===================================================================

	// デバイスのデバッグモードを有効にする
	bool deviceDebugMode = false;
#ifdef _DEBUG
	deviceDebugMode = true;
#endif

	// Direct3D初期化
	std::string errorMsg;
	if (KdDirect3D::Instance().Init(Application::Instance().GetWindowHandle(), w, h, deviceDebugMode, errorMsg) == false) {
		MessageBoxA(Application::Instance().GetWindowHandle(), errorMsg.c_str(), "Direct3D初期化失敗", MB_OK | MB_ICONSTOP);
	}

	// フルスクリーン設定
	if (bFullScreen) {
		HRESULT hr;
		hr = KdDirect3D::Instance().SetFullscreenState(TRUE, 0);
		if (FAILED(hr))
		{
			MessageBoxA(Application::Instance().GetWindowHandle(), "フルスクリーン設定失敗", "Direct3D初期化失敗", MB_OK | MB_ICONSTOP);
		}
	}

	RenderManager::Instance().Init(w, h);
	
	KernelEngine::Init();


	//===================================================================
	// オーディオ初期化
	//===================================================================
	KdAudioManager::Instance().Init();

	return true;
};
void Application::Release()
{
	//imgui解放
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	KdInputManager::Instance().Release();

	KdShaderManager::Instance().Release();

	KdAudioManager::Instance().Release();

	KdDirect3D::Instance().Release();

	KernelEngine::Release();
	// ウィンドウ削除
	m_window.Release();
}