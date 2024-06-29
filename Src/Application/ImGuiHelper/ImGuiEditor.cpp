#include "ImGuiEditor.h"
#include "ImGuiHelper.h"
#include "../Object/Game/GameObject.h"
#include "../Object/Game/Manager/GameObjectManager.h"

void Editor::ImGuiUpdate()
{
	MyImGui::InputText("OpenPath", m_openDirectoryPath);

	ImVec2 childWindowSize = ImGui::GetWindowSize();
	childWindowSize.y /= 0.75f;
	ImGui::BeginChild("DirectoryTree", ImVec2(childWindowSize.x * 0.3, childWindowSize.y));
	{
		DirectoryTree();
	}
	ImGui::EndChild();
	TargetGameObjectSave("");

	m_directoryChanged = false;
	ImGui::SameLine();

	ImGui::BeginChild("DirectoryContents", ImVec2(childWindowSize.x * 0.7, childWindowSize.y));
	{
		DirectoryContents();
	}
	ImGui::EndChild();
	TargetGameObjectSave(m_openDirectoryPath);

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
		ImGui::Button(entry.path().filename().string().c_str());

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
