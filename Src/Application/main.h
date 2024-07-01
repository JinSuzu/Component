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

	HWND GetWindowHandle() const { return m_window.GetWndHandle(); }
	int GetMouseWheelValue() const { return m_window.GetMouseWheelVal(); }
	KdWindow& GetWindow() { return m_window; }

	int GetNowFPS() const { return m_fpsController.m_nowfps; }
	int GetMaxFPS() const { return m_fpsController.m_maxFps; }
	void UpdateFPS() { m_fpsController.UpdateStartTime(); }

	Math::Vector2 GetWindowSize() const { return m_windowSize; }
	Math::Vector2 GetMouse();

	void KdBeginUpdate();
	void PreUpdate();
	void Update();
	void PostUpdate();
	void KdPostUpdate();

	void KdBeginDraw(bool usePostProcess = true);
	void PreDraw();
	void Draw();
	void PostDraw();
	void DrawSprite();
	void KdPostDraw();

	bool GetBuildFlg()const { return m_buildFlg; };
	void TurnBuildFlg() {m_buildFlg = !m_buildFlg; };
	bool GetDebugFlg()const { return m_debugFlg; };

	std::weak_ptr<class Editor> GetEditor() { return m_editor; }
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

	Math::Vector2 m_windowSize;

	
	bool m_buildFlg = true;
	bool m_debugFlg = true;

	std::shared_ptr<class GameObject> m_buildCamera;
	std::shared_ptr<Editor> m_editor;

	void ImGuiProcess();
	void ImGuiUpdate();
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
