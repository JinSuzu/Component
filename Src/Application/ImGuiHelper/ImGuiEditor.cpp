#include "ImGuiEditor.h"
#include "ImGuiHelper.h"

#include "../main.h"

#include "../Object/Game/GameObject.h"
#include "../Object/Game/Manager/GameObjectManager.h"

#include "../SceneBase/Manager/SceneManager.h"

#include "../RenderManger/RenderManger.h"


void Editor::ImGuiUpdate()
{
	MenuBar();
	ImVec2 size = ImGui::GetWindowSize();
	size.x *= 0.9875f;
	size.y -= ImGui::GetItemRectSize().y + 20;
	ImGui::BeginChild("Editor", size);
	{
		ImVec2 size = ImGui::GetWindowSize();
		Hierarchy(ImVec2(size.x * 0.1945, size.y * 0.7)); ImGui::SameLine();
		GameScreen(ImVec2(size.x * 0.6, size.y * 0.7)); ImGui::SameLine();
		Inspector(ImVec2(size.x * 0.1945, size.y * 0.7));
		Prefab(ImVec2(size.x * 0.9995f, size.y * 0.2875));
	}
	ImGui::EndChild();
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
void Editor::Prefab(ImVec2 _size, UINT _flg)
{
	ImGui::BeginChild("Prehab", _size, _flg);
	{
		if (ImGui::BeginTable("Prehab", 2)) {
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::BeginChild("DirectoryTree");
			{
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
	ImGui::EndChild();
}
void Editor::Inspector(ImVec2 _size, UINT _flg)
{
	ImGui::BeginChild("Inspector", _size, _flg);
	{
		if (m_editObject.lock())GameObjectManager::ImGuiGameObject(m_editObject);
	}
	ImGui::EndChild();
}
void Editor::Hierarchy(ImVec2 _size, UINT _flg)
{
	ImGui::BeginChild("##ObjectChild", _size, _flg);
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
void Editor::GameScreen(ImVec2 _size, UINT _flg)
{
	ImGui::BeginChild("OneLine", _size, _flg);
	{//GameScreen

		//Math::Vector2 vec = Math::Vector2(1280, 720) * 0.625;
		Math::Vector2 vec = Math::Vector2(1280, 720);
		static std::shared_ptr<KdTexture> tex;
		tex = RenderManager::Instance().CreateBackBuffer();
		ImGui::Image(tex->WorkSRView(), ImGui::GetWindowSize());
	}
	ImGui::EndChild();
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
	std::filesystem::path dir(m_openDirectoryPath + ".");
	if (!std::filesystem::exists(dir))return;
	for (const auto& entry : std::filesystem::directory_iterator(dir))
	{
		ImGui::TreeNodeEx(entry.path().filename().string().c_str(), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen);

		std::string str = entry.path().extension().string();
		bool flg = entry.is_directory();
		if (str == ".prefab" && SourceGameObjectDataPath(m_openDirectoryPath + entry.path().filename().string()));
		else if (flg)TargetGameObjectSave(m_openDirectoryPath + entry.path().filename().string() + "/");

		if (flg && ImGui::IsItemClicked(0))
		{
			m_openDirectoryPath = m_openDirectoryPath + entry.path().filename().string() + "/";
			m_directoryChanged = true;
		}
	}
}

void Editor::ShowRowTabGroup(int _row)
{
	ImVec2 vec2 = ImGui::GetWindowSize();
	vec2.x /= (float)m_windows[_row].size();
	vec2.y = (vec2.y / (float)m_windows.size()) * (_row ? 0.7f : 1.125);
	if (ImGui::BeginTable("Window", m_windows[_row].size(), ImGuiTableFlags_Resizable)) {
		int column = 0;

		ImGui::TableNextRow(0, vec2.y);
		for (auto& tabGroup : m_windows[_row])
		{
			ImGui::TableSetColumnIndex(column++);
			ImGui::BeginTabBar(("TabGroup" + std::to_string(column)).c_str());
			for (auto& tab : tabGroup.contents)
			{
				if (ImGui::BeginTabItem(tab.c_str()))
				{
					//ImGui::BeginChild((std::to_string(_row) + std::to_string(column)).c_str(), vec2);
					m_editorWindows[tab]();
					//ImGui::EndChild();
					ImGui::EndTabItem();
				}
			}
			ImGui::EndTabBar();
		}
		ImGui::EndTable();
	}
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

