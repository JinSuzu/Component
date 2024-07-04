#include "ImGuiEditor.h"
#include "ImGuiHelper.h"

#include "../main.h"
#include "../Object/Game/GameObject.h"
#include "../Object/Game/Manager/GameObjectManager.h"
#include "../SceneBase/Manager/SceneManager.h"


void Editor::ImGuiUpdate()
{
	ImGui::ShowDemoWindow();
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


	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	ImGui::Begin("DockSpace Demo", &p_open, window_flags);

	ImGui::PopStyleVar(2);

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
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

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
			ImGui::Begin(window.c_str());
			m_editorWindows[window]();
			ImGui::End();
		}
	}
	ImGui::EndChild();

	ImGui::End();
}

void Editor::MenuBar()
{
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
}

bool Editor::SourceGameObject(std::weak_ptr<GameObject> _obj)
{
	bool flg = false;
	if (flg = ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
	{
		ImGui::SetDragDropPayload("GameObject", &_obj, sizeof(*_obj.lock()));
		ImGui::Text("PullGameObject");
		ImGui::EndDragDropSource();
	}
	return flg;
}
void Editor::TargetGameObject(std::weak_ptr<GameObject> _parent)
{
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject"))
		{
			if (payload->DataSize == sizeof(GameObject))
			{
				std::weak_ptr<GameObject> obj = *(std::weak_ptr<GameObject>*)payload->Data;
				obj.lock()->SetUpParent(_parent);
			}
		}

		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObjectDataPath"))
		{

			std::string path = *(std::string*)(payload->Data);
			GameObjectManager::CreateObject(path, _parent);

		}

		ImGui::EndDragDropTarget();
	}
}

#include "../Object/EditorWindow/Base/EditorWindowBase.h"
#include "../Object/EditorWindow/Hierarchy/Hierarchy.h"
#include "../Object/EditorWindow/GameScreen/GameScreen.h"
#include "../Object/EditorWindow/Inspector/Inspector.h"
#include "../Object/EditorWindow/Prefab/Prefab.h"

Editor::Editor()
{
	auto Register
		= [&](std::function<std::shared_ptr<EditorWindowBase>()> _editor)
		{
			std::string tag = PickName(typeid(*_editor().get()).name(),' ');
			m_editorActive[tag] = false;
			m_editorWindows[tag] = _editor;
		};
#define EDITORREGISTER(Tag)													\
	Register([&]()															\
	{																		\
		std::shared_ptr<EditorWindowBase>temp = std::make_shared<Tag>();	\
		temp->SetOwner(this);												\
		return temp;														\
	});

	EDITORREGISTER(Hierarchy)
	EDITORREGISTER(GameScreen)
	EDITORREGISTER(Inspector)
	EDITORREGISTER(Prefab)

	nlohmann::json json = MyJson::InPutJson("Asset/Data/config.ini");
	if (!json.is_null())
	{

	}
	else
	{
		assert(false && "ファイル破損してるよー");
	}

	m_editorActive["Hierarchy"] = true;
	m_editorActive["GameScreen"] = true;
	m_editorActive["Inspector"] = true;
	m_editorActive["Prefab"] = true;
}