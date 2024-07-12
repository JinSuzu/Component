#include "main.h"
#include "Object/Game/Manager/GameObjectManager.h"
#include "Object/Game/GameObject.h"
#include "RenderManger/RenderManger.h"
#include "Utility/Timer.h"
#include "ImGuiHelper/ImGuiHelper.h"
#include "ImGuiHelper/ImGuiEditor.h"
#include "../System/SceneMnager/SceneManager.h"

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
	KdCSVData windowData("Asset/Data/WindowSettings.csv");
	const std::vector<std::string>& sizeData = windowData.GetLine(0);

	//===================================================================
	// 初期設定(ウィンドウ作成、Direct3D初期化など)
	//===================================================================
	//m_windowSize = { (float)atoi(sizeData[0].c_str()) ,(float)atoi(sizeData[1].c_str()) };
	if (Application::Instance().Init(1280, 720) == false) {
		return;
	}

	//===================================================================
	// ゲームループ
	//===================================================================

	// 時間
	m_fpsController.Init();
	// ループ
	while (1)
	{
		// 処理開始時間Get
		m_fpsController.UpdateStartTime();

		std::string titlebar = "Rocket Burst Breaker	fps : " + std::to_string(m_fpsController.m_nowfps);
		SetWindowTextA(GetWindowHandle(), titlebar.c_str());
		
		// ゲーム終了指定があるときはループ終了
		if (m_endFlag)
		{
			break;
		}

		//=========================================
		//
		// ウィンドウ関係の処理
		//
		//=========================================

		// ウィンドウのメッセージを処理する
		m_window.ProcessMessage();

		// ウィンドウが破棄されてるならループ終了
		if (m_window.IsCreated() == false)
		{
			break;
		}

		//=========================================
		//
		// アプリケーション更新処理
		//
		//=========================================
		static bool flg = m_buildFlg || m_debugFlg;
		if (flg != m_buildFlg || m_debugFlg)
		{
			ShowCursor(flg);
			flg = !flg;
		}
		KdBeginUpdate();
		{
			Timer::Instance().PreUpdate();
			GameObjectManager::Instance().PreUpdate();
			GameObjectManager::Instance().Update();
			GameObjectManager::Instance().PostUpdate();

			m_editor->ImGuiUpdate();
		}
		KdPostUpdate();

		//=========================================
		//
		// アプリケーション描画処理
		//
		//=========================================
		RenderManager::Instance().BeginDraw();
		{
			RenderManager::Instance().PreDraw();
			RenderManager::Instance().Draw();
			RenderManager::Instance().PostDraw();
			m_editor->ImGuiDraw();
		}
		RenderManager::Instance().EndDraw();
		//---------------------

		SceneManager::Instance().PushScene();

		//=========================================
		//
		// フレームレート制御
		//
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

void Application::KdBeginUpdate()
{
	// 入力状況の更新
	KdInputManager::Instance().Update();

	// 空間環境の更新
	KdShaderManager::Instance().WorkAmbientController().Update();
}
void Application::KdPostUpdate()
{
	// 3DSoundListnerの行列を更新
	KdAudioManager::Instance().SetListnerMatrix(KdShaderManager::Instance().GetCameraCB().mView.Invert());
}

void Application::KdBeginDraw(bool usePostProcess)
{
	KdDirect3D::Instance().ClearBackBuffer();
	KdShaderManager::Instance().m_postProcessShader.Draw();

	if (!usePostProcess) return;
}


bool Application::Init(int w, int h)
{
	//===================================================================
	// ウィンドウ作成
	//===================================================================
	if (!m_window.IsCreated() && m_window.Create(w, h, "Rocket Burst Breaker", "Window") == false) {
		MessageBoxA(nullptr, "ウィンドウ作成に失敗", "エラー", MB_OK);
		return false;
	}

	//===================================================================
	// シェーダー初期化
	//===================================================================
	RenderManager::Instance().Init();
	m_editor = std::make_shared<Editor>();

	//===================================================================
	// オーディオ初期化
	//===================================================================
	KdAudioManager::Instance().Init();

	SceneManager::Instance().Init();

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

	// ウィンドウ削除
	m_window.Release();
}