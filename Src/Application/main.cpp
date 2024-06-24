#include "main.h"
#include "SceneBase/Manager/SceneManager.h"
#include "SceneBase/SceneBase.h"
#include "Object/Game/Manager/GameObjectManager.h"
#include "Object/Game/GameObject.h"
#include "RenderManger/RenderManger.h"
#include "Utility/Timer.h"


// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// エントリーポイント
// アプリケーションはこの関数から進行する
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
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

Math::Vector2 Application::GetMouse()
{
	POINT mouse;
	//ディスプレイの左上が(0,0)
	GetCursorPos(&mouse);

	ScreenToClient(m_window.GetWndHandle(), &mouse);

	mouse.x -= (long)(m_windowSize.x / 2.0f);
	mouse.y -= (long)(m_windowSize.y / 2.0f);

	return { (float)mouse.x,-(float)mouse.y };
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// アプリケーション更新開始
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void Application::KdBeginUpdate()
{
	// 入力状況の更新
	KdInputManager::Instance().Update();

	// 空間環境の更新
	KdShaderManager::Instance().WorkAmbientController().Update();
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// アプリケーション更新終了
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void Application::KdPostUpdate()
{
	// 3DSoundListnerの行列を更新
	KdAudioManager::Instance().SetListnerMatrix(KdShaderManager::Instance().GetCameraCB().mView.Invert());
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// アプリケーション更新の前処理
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void Application::PreUpdate()
{
	Timer::Instance().PreUpdate();
	SceneManager::Instance().PreUpdate();
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// アプリケーション更新
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void Application::Update()
{
	SceneManager::Instance().Update();
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// アプリケーション更新の後処理
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void Application::PostUpdate()
{
	SceneManager::Instance().PostUpdate();
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// アプリケーション描画開始
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void Application::KdBeginDraw(bool usePostProcess)
{
	KdDirect3D::Instance().ClearBackBuffer();

	if (!usePostProcess) return;
	KdShaderManager::Instance().m_postProcessShader.Draw();
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// アプリケーション描画終了
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void Application::KdPostDraw()
{
	// BackBuffer -> 画面表示
	KdDirect3D::Instance().WorkSwapChain()->Present(0, 0);
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// アプリケーション描画の前処理
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void Application::PreDraw()
{
	RenderManager::Instance().PreDraw();
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// アプリケーション描画
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void Application::Draw()
{
	RenderManager::Instance().Draw();
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// アプリケーション描画の後処理
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void Application::PostDraw()
{
	RenderManager::Instance().DrawDebug();
	// 画面のぼかしや被写界深度処理の実施
	KdShaderManager::Instance().m_postProcessShader.PostEffectProcess();
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 2Dスプライトの描画
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void Application::DrawSprite()
{
	//SceneManager::Instance().DrawSprite();
	RenderManager::Instance().DrawSprite();
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// アプリケーション初期設定
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
bool Application::Init(int w, int h)
{
	//===================================================================
	// ウィンドウ作成
	//===================================================================
	if (m_window.Create(w, h, "Rocket Burst Breaker", "Window") == false) {
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
	}*/

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
	if (KdDirect3D::Instance().Init(m_window.GetWndHandle(), w, h, deviceDebugMode, errorMsg) == false) {
		MessageBoxA(m_window.GetWndHandle(), errorMsg.c_str(), "Direct3D初期化失敗", MB_OK | MB_ICONSTOP);
		return false;
	}

	// フルスクリーン設定
	if (bFullScreen) {
		HRESULT hr;

		hr = KdDirect3D::Instance().SetFullscreenState(TRUE, 0);
		if (FAILED(hr))
		{
			MessageBoxA(m_window.GetWndHandle(), "フルスクリーン設定失敗", "Direct3D初期化失敗", MB_OK | MB_ICONSTOP);
			return false;
		}
	}

	//===================================================================
	// シェーダー初期化
	//===================================================================
	KdShaderManager::Instance().Init();

	//===================================================================
	// オーディオ初期化
	//===================================================================
	KdAudioManager::Instance().Init();

	//===================================================================
	// imgui初期設定
	//===================================================================
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	// Setup Dear ImGui style
	ImGui::StyleColorsClassic();
	// Setup Platform/Renderer bindings
	ImGui_ImplWin32_Init(m_window.GetWndHandle());
	ImGui_ImplDX11_Init(KdDirect3D::Instance().WorkDev(), KdDirect3D::Instance().WorkDevContext());
	{
		// 日本語対応
#include "imgui/ja_glyph_ranges.h"
		ImGuiIO& io = ImGui::GetIO();
		ImFontConfig config;
		config.MergeMode = true;
		io.Fonts->AddFontDefault();
		io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\msgothic.ttc", 13.0f, &config, glyphRangesJapanese);
	}

	m_buildCamera = GameObjectManager::CreateObject(std::string("BuildCamera"), std::weak_ptr<GameObject>(), false);
	SceneManager::Instance().Init();

	return true;
};

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// アプリケーション実行
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void Application::Execute()
{
	KdCSVData windowData("Asset/Data/WindowSettings.csv");
	const std::vector<std::string>& sizeData = windowData.GetLine(0);

	//===================================================================
	// 初期設定(ウィンドウ作成、Direct3D初期化など)
	//===================================================================
	m_windowSize = { (float)atoi(sizeData[0].c_str()) ,(float)atoi(sizeData[1].c_str()) };
	if (Application::Instance().Init((int)m_windowSize.x, (int)m_windowSize.y) == false) {
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

		//if (GetAsyncKeyState(VK_ESCAPE))
		//{
		//	//			if (MessageBoxA(m_window.GetWndHandle(), "本当にゲームを終了しますか？",
		//	//				"終了確認", MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2) == IDYES)
		//	{
		//		End();
		//	}
		//}
		//=========================================
		//
		// アプリケーション更新処理
		//
		//=========================================

		ShowCursor(m_buildFlg || m_debugFlg);
		KdBeginUpdate();
		{
			m_buildCamera->SetActive(m_buildFlg);
			m_buildCamera->PreUpdate();
			m_buildCamera->Update();
			m_buildCamera->PostUpdate();

			PreUpdate();
			Update();
			PostUpdate();
		}
		KdPostUpdate();

		//=========================================
		//
		// アプリケーション描画処理
		//
		//=========================================
		KdBeginDraw();
		{
			//Draw();
			RenderManager::Instance().PreDraw();
			RenderManager::Instance().Draw();

			PostDraw();

			DrawSprite();

			ImGuiProcess();
		}
		KdPostDraw();
		//---------------------


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

// アプリケーション終了
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

void Application::ImGuiProcess()
{
	if (GetAsyncKeyState(VK_UP) & 0x8000)m_debugFlg = true;
	else if (GetAsyncKeyState(VK_DOWN) & 0x8000)m_debugFlg = false;

	if (!m_debugFlg)return;

	// ImGui開始
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	// ImGui処理
	ImGuiUpdate();
	//リリース時はImGuiの部分は通らないようにする
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void Application::ImGuiUpdate()
{
	//ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiSetCond_Once);
	//ImGui::SetNextWindowSize(ImVec2(500, 300), ImGuiSetCond_Once);
	// デバッグウィンドウ
	ImGui::ShowDemoWindow(nullptr);
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
	ImGui::Begin("Fullscreen Window", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
	{
		//ImGui::Begin("Debug Window", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
		UINT childFlg = ImGuiChildFlags_AlwaysAutoResize | ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY;


		{//MenuBar
			ImGui::Text("FPS : %d", m_fpsController.m_nowfps); ImGui::SameLine();
			ImGui::Text("%.2f,%.2f", GetMouse().x, GetMouse().y); ImGui::SameLine();

			if (ImGuiWindowCenterButton(m_buildFlg ? "StartRun" : "StartBuild"))
			{
				SceneManager::Instance().ReLoad();
				m_buildFlg = !m_buildFlg;
			}

			if (m_buildFlg)
			{
				if (GetAsyncKeyState(VK_F5) & 0x8000)
				{
					SceneManager::Instance().ReLoad();
					m_buildFlg = !m_buildFlg;
				}
			}
			else
			{
				if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
				{
					SceneManager::Instance().ReLoad();
					m_buildFlg = !m_buildFlg;
				}

			}

			//ImGui::Text("ThreadMax : %d", std::hardware_constructive_interference_size); ImGui::SameLine();
		}
		ImGui::BeginChild("OneLine", ImVec2(), childFlg);
		{
			ImGui::BeginChild("OneLine", ImVec2(), childFlg);
			{//GameScreen

				Math::Vector2 vec = Math::Vector2(1280, 720) * 0.625;
				static std::shared_ptr<KdTexture> tex;
				tex = RenderManager::Instance().CreateBackBuffer();
				ImGui::Image(tex->WorkSRView(), ImVec2(vec.x, vec.y));
			}
			ImGui::EndChild();

			ImGui::BeginChild("Scene", ImVec2(), ImGuiChildFlags_Border | childFlg);
			{//Scene
				ImGui::InputText("ScenePhase", &SceneManager::Instance().RefScenePhase());
				SceneManager::Instance().ImGuiUpdate();
				int SceneNum = SceneManager::Instance().GetNowSceneNum();
				if (ImGui::SliderInt("SceneNum", &SceneNum, 0, SceneID::Max - 1)) SceneManager::Instance().ShiftScene((SceneID)SceneNum);
				if (ImGui::Button("ReLoad"))SceneManager::Instance().ReLoad();
			}
			ImGui::EndChild();

			ImGui::SameLine();
			ImGui::BeginChild("CreateObject", ImVec2(), ImGuiChildFlags_Border | childFlg);
			{//PreHubもどき
				SceneManager::Instance().m_objectMgr->ImGuiCreateObject();
			}
			ImGui::EndChild();
		}
		ImGui::EndChild();

		{//ObjectEditor
			ImGui::SameLine();
			ImGui::BeginChild("Editor", ImVec2(450, 680), ImGuiWindowFlags_NoResize);
			//GameObjectManager::ImGuiGameObject(m_buildCamera);
			SceneManager::Instance().m_objectMgr->ImGuiUpdate();
			ImGui::EndChild();
		}
		/*if (ImGui::BeginTabBar("CreateObject"))
		{
			if (ImGui::BeginTabItem("Object"))
			{
				if (ImGui::TreeNode("Scene"))
				{
					ImGui::TreePop();
				}
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Other"))
			{


				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}*/

	}
	ImGui::End();
}
