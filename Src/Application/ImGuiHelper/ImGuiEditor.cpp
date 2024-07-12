#include "ImGuiEditor.h"
#include "ImGuiHelper.h"

#include "../main.h"
#include "../Object/Game/GameObject.h"
#include "../Object/Game/Manager/GameObjectManager.h"
#include "../SceneBase/Manager/SceneManager.h"
#include "../Object/EditorWindow/Base/EditorWindowBase.h"


void Editor::ImGuiDraw()
{
	if (!Application::Instance().GetDebugFlg())return;
	//リリース時はImGuiの部分は通らないようにする
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
}
void Editor::ImGuiUpdate()
{
	if (!Application::Instance().GetDebugFlg())return;
	if (GetAsyncKeyState(VK_UP) & 0x8000)Application::Instance().SetDebugFlg(true);
	else if (GetAsyncKeyState(VK_DOWN) & 0x8000)Application::Instance().SetDebugFlg(false);

	// ImGui開始
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	
	// デバッグウィンドウ
	static bool p_open = true;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	window_flags |= ImGuiWindowFlags_NoBackground;
	ImGui::Begin("DockSpace Demo", &p_open, window_flags);

	ImGui::PopStyleVar(2);

	MenuBar();

	ImGui::BeginChild("EditorSpace", ImGui::GetContentRegionAvail(), ImGuiChildFlags_None, ImGuiWindowFlags_NoDocking);
	{
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		for (auto& window : m_windowList)
		{
			window->Draw();
		}
	}
	ImGui::EndChild();

	ImGui::End();
}

void Editor::MenuBar()
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open"));
			if (ImGui::MenuItem("Save"));
			if (ImGui::MenuItem("Save as"));
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

	ImGui::Text("FPS : %d", Application::Instance().GetNowFPS()); ImGui::SameLine();

	ImGui::SameLine();
	if (MyImGui::ButtonWindowCenter(Application::Instance().GetBuildFlg() ? "StartRun" : "StartBuild"))
	{
		SceneManager::Instance().ReLoad();
		Application::Instance().TurnBuildFlg();
	}
	if (GetAsyncKeyState(Application::Instance().GetBuildFlg() ? VK_F5 : VK_ESCAPE) & 0x8000)
	{
		SceneManager::Instance().ReLoad();
		Application::Instance().TurnBuildFlg();
	}
}

void Editor::OverwriteWindow()
{
	m_windowList.clear();
	for (auto& map : m_editorActive)
	{
		if (map.second)
		{
			m_windowList.push_back(m_editorWindows[map.first]());
		}
	}
}

#include "../Object/EditorWindow/Hierarchy/Hierarchy.h"
#include "../Object/EditorWindow/GameScreen/GameScreen.h"
#include "../Object/EditorWindow/Inspector/Inspector.h"
#include "../Object/EditorWindow/Prefab/Prefab.h"
#include "../Object/EditorWindow/DebugLog/DebugLog.h"
void Editor::Init()
{
	//===================================================================
	// imgui初期設定
	//===================================================================
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	// Setup Dear ImGui style
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // キーボードナビゲーションを有効にする
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // ドッキングを有効にする
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // マルチビューポートを有効にする
	ImGui::StyleColorsClassic();

	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	// Setup Platform/Renderer bindings
	ImGui_ImplWin32_Init(Application::Instance().GetWindowHandle());
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

	//===================================================================
	// Editor初期設定
	//===================================================================
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
		return temp;														\
	})

	EDITORREGISTER(Hierarchy);
	EDITORREGISTER(GameScreen);
	EDITORREGISTER(Inspector);
	EDITORREGISTER(Prefab);
	EDITORREGISTER(DebugLog);

	nlohmann::json json = MyJson::InPutJson("Asset/Data/config.ini");
	if (!json.is_null())
	{
		auto it = json.begin();
		while (it != json.end())
		{
			m_editorActive[it.key()] = *it;
			it++;
		}
	}
	else
	{
		Application::Instance().m_log.AddLog("not found \"config\" by EditorWindow");
		m_editorActive["Hierarchy"] = true;
		m_editorActive["GameScreen"] = true;
		m_editorActive["Inspector"] = true;		
		m_editorActive["Prefab"] = true;
	}


	OverwriteWindow();
}

void Editor::Release()
{
	nlohmann::json json;
	for (auto& key : m_editorActive)json[key.first] = key.second;
	MyJson::OutPutJson(json, "Asset/Data/config.ini");
}