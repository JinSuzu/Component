#include "EditorWindowManager.h"

#include "../../../Application/main.h"
#include "../../../Application/Object/Game/GameObject.h"
#include "../../../Application/Object/Game/Manager/GameObjectManager.h"
#include "../../EditorWindow/Base/EditorWindowBase.h"

void Editor::ImGuiDraw()
{
	if (!KernelEngine::is_Debugging())return;

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		KdDirect3D::Instance().SetBackBuffer();
	}
}
void Editor::ImGuiUpdate()
{
	if (GetAsyncKeyState(VK_UP) & 0x8000)KernelEngine::SetDebugging(true);
	else if (GetAsyncKeyState(VK_DOWN) & 0x8000)KernelEngine::SetDebugging(false);
	if (!KernelEngine::is_Debugging())return;

	// ImGui開始
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();

	// デバッグウィンドウ
	static bool p_open = true;


	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());
	ImGui::Begin("DockSpace Demo", &p_open, window_flags);
	ImGui::PopStyleVar(1);

	MenuBar();
	PlayButton();

	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGui::DockSpace(ImGui::GetID("DockSpace Demo"));
	}

	for (auto& window : m_windowList)
	{
		window->Update();
	}


	ImGui::End();
}

void Editor::MenuBar()
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open"))SceneManager::Instance().SaveScene();
			if (ImGui::MenuItem("Save"))SceneManager::Instance().SaveScene();
			if (ImGui::MenuItem("Save as"))SceneManager::Instance().SaveAsScene();
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Window"))
		{
			bool flg = false;
			for (auto& item : m_editorActive)
			{
				if (ImGui::MenuItem(item.first.c_str(), "", item.second))
				{
					flg = true;
					item.second = !item.second;
				}
			}
			if (flg) OverwriteWindow();
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
}
void Editor::PlayButton()
{
	ImVec2 defaultSpacing = ImGui::GetStyle().ItemSpacing;
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(defaultSpacing.x, 1.0f)); // 垂直方向の間隔を2.0fに設定

	ImGui::BeginGroup();
	{
		Utility::ImGuiHelper::SetCenterCursorWidth();
		KernelEngine::StartButton();
		ImGui::SameLine();
		KernelEngine::StopButton();
	}
	ImGui::EndGroup();

	ImGui::PopStyleVar();
}
void Editor::OverwriteWindow()
{
	if (!m_editorActive.empty())ReleaseWindows();
	for (auto& map : m_editorActive)
	{
		if (map.second)
		{
			m_windowList.push_back(m_editorWindows[map.first]());
		}
	}
}

#include "../../EditorWindow/Hierarchy/Hierarchy.h"
#include "../../EditorWindow/GameScreen/GameScreen.h"
#include "../../EditorWindow/Inspector/Inspector.h"
#include "../../EditorWindow/Prefab/Prefab.h"
#include "../../EditorWindow/DebugLog/DebugLog.h"
#include "../../EditorWindow/SetUpScene/SetUpScene.h"
void Editor::Init()
{
	//===================================================================
	// imgui初期設定
	//===================================================================
	{
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		// Setup Dear ImGui style
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // ドッキングを有効にする
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // マルチビューポートを有効にする
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // キーボードナビゲーションを有効にする
		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		// Setup Platform/Renderer bindings
		ImGui_ImplWin32_Init(Application::Instance().GetWindowHandle());
		ImGui_ImplDX11_Init(KdDirect3D::Instance().WorkDev(), KdDirect3D::Instance().WorkDevContext());
		{
			// 日本語対応
#include "imgui/ja_glyph_ranges.h"
			ImFontConfig config;
			config.MergeMode = true;
			io.Fonts->AddFontDefault();
			io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\msgothic.ttc", 13.0f, &config, glyphRangesJapanese);
		}
	}

	//===================================================================
	// Editor初期設定
	//===================================================================
	{
		auto Register
			= [&](std::string _name, std::function<std::shared_ptr<EditorWindowBase>()> _editor)
			{
				m_editorActive[_name] = false;
				m_editorWindows[_name] = _editor;
			};
#define EDITORREGISTER(Tag)													\
	Register(#Tag , [&]()													\
	{																		\
		std::shared_ptr<EditorWindowBase>temp = std::make_shared<Tag>();	\
		temp->SetOwner(this);												\
		temp->SetName(#Tag);												\
		temp->Init();														\
		return temp;														\
	})

		EDITORREGISTER(Hierarchy);
		EDITORREGISTER(GameScreen);
		EDITORREGISTER(Inspector);
		EDITORREGISTER(Prefab);
		EDITORREGISTER(SetUpScene);
		Register("DebugLog", [&]() { return m_debugLog; });

		ConfigManger config;
		if (config.Load("Editor"))
		{
			auto it = config.begin();
			while (it != config.end())
			{
				m_editorActive[it.key()] = *it;
				it++;
			}
		}
		else
		{
			m_editorActive["Hierarchy"] = true;
			m_editorActive["GameScreen"] = true;
			m_editorActive["Inspector"] = true;
			m_editorActive["Prefab"] = true;
		}

		m_debugLog = std::make_shared<DebugLog>();
		m_debugLog->SetOwner(this);
		m_debugLog->SetName("DebugLog");

		OverwriteWindow();
	}
}

void Editor::ReleaseWindows()
{
	std::list<std::shared_ptr<EditorWindowBase>>::iterator it = m_windowList.begin();
	while (it != m_windowList.end())
	{
		(*it)->Release();
		it = m_windowList.erase(it);
	}
}
void Editor::Release()
{
	ReleaseWindows();

	ConfigManger config;
	for (auto& key : m_editorActive)config[key.first] = key.second;
	config.Save("Editor");
}