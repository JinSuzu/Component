#pragma once
//============================================================
// アプリケーションクラス
//	APP.～ でどこからでもアクセス可能
//============================================================
class Application
{
	// メンバ
public:

	// アプリケーション実行
	void Execute();

	// アプリケーション終了
	void End() { m_endFlag = true; }

	HWND GetWindowHandle()	const { return m_window.GetWndHandle(); }
	int GetMouseWheelValue()const { return m_window.GetMouseWheelVal(); }
	KdWindow& GetWindow() { return m_window; }

	int GetNowFPS() const { return m_fpsController.m_nowfps; }
	int GetMaxFPS() const { return m_fpsController.m_maxFps; }
	void UpdateFPS() { m_fpsController.UpdateStartTime(); }

	const int* GetWindowSize() { return KdDirect3D::Instance().GetWindowSize(); }
	const int* GetGameWindowSize() { return m_windowSize; }

	Math::Vector2 GetMouse();
	const Math::Vector2& GetMouseDelta() const { return m_mouseDelta; };
private:
	// アプリケーション初期化
	bool Init(int w, int h);

	// アプリケーション解放
	void Release();

	// ゲームウィンドウクラス
	KdWindow		m_window;

	// FPSコントローラー
	KdFPSController	m_fpsController;

	// ゲーム終了フラグ trueで終了する
	bool		m_endFlag = false;


	Math::Vector2 m_mouseDelta;
	int           m_windowSize[2] = { 0,0 };
	//=====================================================
	// シングルトンパターン
	//=====================================================
private:

	Application() {}


public:
	static Application& Instance() {
		static Application Instance;
		return Instance;
	}
};
