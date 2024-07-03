#include "ImGuiEditor.h"
#include "ImGuiHelper.h"

#include "../main.h"

#include "../Object/Game/GameObject.h"
#include "../Object/Game/Manager/GameObjectManager.h"

#include "../SceneBase/Manager/SceneManager.h"
#include "../RenderManger/RenderManger.h"
#include "../AssetManager/AssetManager.h"



void Editor::ImGuiUpdate()
{
	// デバッグウィンドウ
	static bool p_open = true;
	static bool opt_fullscreen = true;
	static bool opt_padding = false;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}
	else
	{
		dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
	}

	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	if (!opt_padding)
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", &p_open, window_flags);
	if (!opt_padding)
		ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// Submit the DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Options"))
		{
			ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
			ImGui::MenuItem("Padding", NULL, &opt_padding);
			ImGui::Separator();

			if (ImGui::MenuItem("Flag: NoDockingOverCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingOverCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingOverCentralNode; }
			if (ImGui::MenuItem("Flag: NoDockingSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingSplit; }
			if (ImGui::MenuItem("Flag: NoUndocking", "", (dockspace_flags & ImGuiDockNodeFlags_NoUndocking) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoUndocking; }
			if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
			if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
			if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
			ImGui::Separator();

			if (ImGui::MenuItem("Close", NULL, false))
				p_open = false;
			ImGui::EndMenu();
		}
		MenuBar();
		ImGui::EndMenuBar();
	}


	for (auto& tab : m_tabList)
	{
		ImGui::Begin(tab.c_str());
		m_editorWindows[tab]();
		ImGui::End();
	}
	ImGui::End();
}

void Editor::MenuBar()
{
	ImGui::Text("FPS : %d", Application::Instance().GetNowFPS()); ImGui::SameLine();
	//ImGui::Text("%.2f,%.2f", GetMouse().x, GetMouse().y); ImGui::SameLine();
	/*if (ImGui::SmallButton("Timer"))ImGui::OpenPopup("Timer");
	if (ImGui::BeginPopup("Timer"))
	{
		Timer::Instance().ImGuiUpdate();
		ImGui::EndPopup();
	}*/

	ImGui::SameLine();
	if (MyImGui::SmallButtonWindowCenter(Application::Instance().GetBuildFlg() ? "StartRun" : "StartBuild"))
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
void Editor::Prefab()
{
	if (ImGui::BeginTable("Prehab", 2)) {
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::BeginChild("DirectoryTree");
		{
			if (ImGui::TreeNode("favorite"))
			{
				for (auto& set : m_favoritePathList)DirectoryTree(set.second,set.first);
				ImGui::TreePop();
			}
			DirectoryTree();
		}
		ImGui::EndChild();
		TargetGameObjectSave("");

		m_directoryChanged = false;

		ImGui::TableSetColumnIndex(1);
		ImGui::BeginChild("DirectoryContents");
		{
			DirectoryContents();
		}
		ImGui::EndChild();
		TargetGameObjectSave(m_openDirectoryPath);

		ImGui::EndTable();
	}
}
void Editor::Inspector()
{

	if (m_editObject.lock())GameObjectManager::ImGuiGameObject(m_editObject);

}
void Editor::Hierarchy()
{
	ImGui::BeginChild("##ObjectChild");
	{
		SceneManager::Instance().m_objectMgr->ImGuiUpdate();
	}
	ImGui::EndChild();
	if (ImGui::IsItemClicked(1))ImGui::OpenPopup("CreateObject");
	Editor::TargetGameObject(std::weak_ptr<GameObject>());
	if (ImGui::BeginPopup("CreateObject"))
	{
		if (m_editObject.lock())
		{
			static std::string path;
			ImGui::InputText("##Path", &path);
			if (ImGui::SameLine(); ImGui::Button("Save"))
			{
				nlohmann::json json = nlohmann::json::array();
				json.push_back(m_editObject.lock()->OutPutFamilyJson());
				MyJson::OutPutJson(json, path);
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::MenuItem("Remove"))m_editObject.lock()->Destroy();
		}
		if (ImGui::Button("Create"))GameObjectManager::CreateObject(std::string(), m_editObject);
		ImGui::EndPopup();
	}

}
void Editor::GameScreen()
{
	//Math::Vector2 vec = Math::Vector2(1280, 720) * 0.625;
	Math::Vector2 vec = Math::Vector2(1280, 720);
	static std::shared_ptr<KdTexture> tex;
	tex = RenderManager::Instance().CreateBackBuffer();
	ImGui::Image(tex->WorkSRView(), ImGui::GetWindowSize());
}

void Editor::DirectoryTree()
{
	std::filesystem::path dir(".");
	for (const auto& entry : std::filesystem::directory_iterator(dir))
	{
		if (entry.is_directory())
		{
			DirectoryTree(entry.path().filename().string(), std::string());
		}
	}
}
void Editor::DirectoryTree(std::string _name, std::string _path)
{
	std::string path = _path + _name + "/";
	std::filesystem::path dir(path + ".");

	std::list<std::string> nameList;
	for (const auto& entry : std::filesystem::directory_iterator(dir))
	{
		if (entry.is_directory())
		{
			nameList.push_back(entry.path().filename().string());
		}
	}

	ImGui::PushStyleColor(ImGuiCol_Text, m_openDirectoryPath == path ? ImVec4(0.5f, 0.5f, 1.0f, 1.0f) : ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
	int treeFlg = ImGuiTreeNodeFlags_OpenOnArrow;
	if (nameList.empty())treeFlg |= ImGuiTreeNodeFlags_Leaf;
	if (m_openDirectoryPath == path)m_directoryChanged = false;
	if (m_directoryChanged && m_openDirectoryPath.find(path) == 0) ImGui::SetNextItemOpen(true);

	bool open = ImGui::TreeNodeEx((_name).c_str(), treeFlg);
	if (!ImGui::IsItemToggledOpen() && ImGui::IsItemClicked())m_openDirectoryPath = _path + _name + "/";
	TargetGameObjectSave(_path + _name + "/");


	if (open)
	{
		for (const auto& entry : nameList)
		{
			DirectoryTree(entry, path);
		}
		ImGui::TreePop();
	}


	ImGui::PopStyleColor();
}
void Editor::DirectoryContents()
{
	static std::shared_ptr<KdTexture> IconDocumentIcon = AssetManager::Instance().GetKdTexture("Asset/Textures/UI/DocumentIcon.png");
	static std::shared_ptr<KdTexture> CloseFolderIcon = AssetManager::Instance().GetKdTexture("Asset/Textures/UI/CloseFolderIcon.png");
	//static std::shared_ptr<KdTexture> OpenFolderIcon = AssetManager::Instance().GetKdTexture("Asset/Textures/UI/ OpenFolderIcon.png");

	static float thumbnailSize = 64.0f;
	static float padding = 16.0f;
	float cellSize = thumbnailSize + padding;

	float panelWidth = ImGui::GetContentRegionAvail().x;
	int columnCount = (int)(panelWidth / cellSize);
	if (columnCount < 1)columnCount = 1;

	ImGui::Columns(columnCount, 0, false);

	std::filesystem::path dir(m_openDirectoryPath + ".");
	if (!std::filesystem::exists(dir))return;
	for (const auto& entry : std::filesystem::directory_iterator(dir))
	{
		ImGui::ImageButton((entry.is_directory() ? CloseFolderIcon : IconDocumentIcon)->WorkSRView(), { thumbnailSize ,thumbnailSize });

		std::string str = entry.path().extension().string();
		bool flg = entry.is_directory();
		if (str == ".prefab" && SourceGameObjectDataPath(m_openDirectoryPath + entry.path().filename().string()));
		else if (flg)TargetGameObjectSave(m_openDirectoryPath + entry.path().filename().string() + "/");

		if (flg && ImGui::IsItemClicked(0))
		{
			m_openDirectoryPath = m_openDirectoryPath + entry.path().filename().string() + "/";
			m_directoryChanged = true;
		}
		ImGui::TextWrapped(entry.path().filename().string().c_str());
		ImGui::NextColumn();
	}
	ImGui::Columns();
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

void Editor::TargetGameObjectSave(std::string _path)
{
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject"))
		{
			IM_ASSERT(payload->DataSize == sizeof(GameObject));
			std::weak_ptr<GameObject> obj = *(std::weak_ptr<GameObject>*)payload->Data;
			MyJson::OutPutJson(obj.lock()->OutPutFamilyJson(), _path + obj.lock()->GetName() + ".prefab");
		}
		ImGui::EndDragDropTarget();
	}
}

bool Editor::SourceGameObjectDataPath(std::string _path)
{
	bool flg = false;
	if (flg = ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
	{
		static std::string path = _path;
		path = _path;
		ImGui::SetDragDropPayload("GameObjectDataPath", &path, sizeof(path));
		ImGui::Text("PullGameObjectDataPath");
		ImGui::EndDragDropSource();
	}
	return flg;
}

