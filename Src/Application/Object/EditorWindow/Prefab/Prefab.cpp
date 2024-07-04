#include "Prefab.h"
#include "../../../AssetManager/AssetManager.h"
#include "../../../ImGuiHelper/ImGuiEditor.h"
#include "../../Game/GameObject.h"
void Prefab::Update()
{
	if (ImGui::BeginTable("Prehab", 2,ImGuiTableFlags_Resizable)) {
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::BeginChild("DirectoryTree");
		{
			if (ImGui::TreeNode("favorite"))
			{
				for (auto& set : m_favoritePathList)DirectoryTree(set.second, set.first);
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

void Prefab::DirectoryTree()
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
void Prefab::DirectoryTree(std::string _name, std::string _path)
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
void Prefab::DirectoryContents()
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

void Prefab::TargetGameObjectSave(std::string _path)
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
bool Prefab::SourceGameObjectDataPath(std::string _path)
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

