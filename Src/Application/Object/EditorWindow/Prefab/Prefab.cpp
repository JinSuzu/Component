#include "Prefab.h"
#include "../../../main.h"
#include "../../../AssetManager/AssetManager.h"
#include "../../../ImGuiHelper/ImGuiEditor.h"
#include "../../../ImGuiHelper/ImGuiHelper.h"
#include "../../Game/GameObject.h"
#include "../../Game/Manager/GameObjectManager.h"

void Prefab::UpdateContents()
{
	if (ImGui::BeginTable("Prehab", 2, ImGuiTableFlags_Resizable)) {
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);

		ImGui::BeginChild("DirectoryTree");
		{
			if (ImGui::TreeNode("favorite"))
			{
				for (auto& set : m_favoritePathList)DirectoryTree(set);
				ImGui::TreePop();
			}
			for (const auto& entry : std::filesystem::directory_iterator(std::filesystem::path(".")))
			{
				if (entry.is_directory())
				{
					DirectoryTree(entry.path());
				}
			}
		}
		ImGui::EndChild();
		MyDragDrop::TargetGameObjectDataSave("");

		m_directoryChanged = false;
		ImGui::TableSetColumnIndex(1);

		ImGui::BeginChild("DirectoryContents");
		{
			DirectoryContents();
		}
		ImGui::EndChild();
		EditFile(m_openDirectoryPath);
		MyDragDrop::TargetGameObjectDataSave(m_openDirectoryPath.string());

		ImGui::EndTable();
	}

	m_bOpenFileEditor = false;
}

void Prefab::DirectoryTree(const std::filesystem::path& dir)
{
	//自分が持っているディレクトリを洗い出す
	std::vector<std::filesystem::path> subdirs;
	for (const auto& entry : std::filesystem::directory_iterator(dir))
	{
		if (entry.is_directory())
		{
			subdirs.push_back(entry.path());
		}
	}

	//閲覧ディレクトリが変更された時、その階層までのTreeを開く奴
	if (m_openDirectoryPath == dir) m_directoryChanged = false;
	if (m_directoryChanged && m_openDirectoryPath.string().find(dir.string()) == 0) ImGui::SetNextItemOpen(true);

	//ディレクトリの下にディレクトリが無い場合の処理
	ImGuiTreeNodeFlags treeFlg = ImGuiTreeNodeFlags_OpenOnArrow | (subdirs.empty() ? ImGuiTreeNodeFlags_Leaf : 0);

	//閲覧中のディレクトリの強調表示
	ImGui::PushStyleColor(ImGuiCol_Text, m_openDirectoryPath == dir ? ImVec4(0.5f, 0.5f, 1.0f, 1.0f) : ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
	bool open = ImGui::TreeNodeEx(dir.filename().string().c_str(), treeFlg);
	ImGui::PopStyleColor();

	//階層移動
	EditFile(dir);
	//Pathを抜き出す
	MyDragDrop::TargetGameObjectDataSave(dir.string());

	if (open)
	{
		//洗い出したディレクトリを再帰的に表示
		for (const auto& subdir : subdirs)
		{
			DirectoryTree(subdir);
		}
		ImGui::TreePop();
	}

}
void Prefab::DirectoryContents()
{
	static std::shared_ptr<KdTexture> IconDocumentIcon = AssetManager::Instance().GetKdTexture("Asset/Textures/UI/DocumentIcon.png");
	static std::shared_ptr<KdTexture> CloseFolderIcon = AssetManager::Instance().GetKdTexture("Asset/Textures/UI/CloseFolderIcon.png");
	//static std::shared_ptr<KdTexture> OpenFolderIcon = AssetManager::Instance().GetKdTexture("Asset/Textures/UI/ OpenFolderIcon.png");

	static float thumbnailSize = 64.0f;
	static float padding = 16.0f;
	float cellSize = thumbnailSize + padding;

	//横に表示できる最大数を出す
	float panelWidth = ImGui::GetContentRegionAvail().x;
	int columnCount = (int)(panelWidth / cellSize);
	if (columnCount < 1)columnCount = 1;
	//計算した表示数に改行までの許容数をする
	ImGui::Columns(columnCount, 0, false);

	//存在しないパスである場合はリターン
	if (!std::filesystem::exists(m_openDirectoryPath))return;

	for (const auto& entry : std::filesystem::directory_iterator(m_openDirectoryPath))
	{
		bool is_directory = entry.is_directory();
		std::string filename = entry.path().filename().string();

		ImGui::PushID(filename.c_str());
		ImGui::ImageButton((is_directory ? CloseFolderIcon : IconDocumentIcon)->WorkSRView(), { thumbnailSize ,thumbnailSize });
		ImGui::PopID();

		std::filesystem::path itemPath = std::filesystem::relative(entry.path()).string();

		//ディレクトリ内のファイルの拡張子に合わせたソースを呼び出す
		std::unordered_map<std::string, std::function<bool(const std::filesystem::path&)>>::iterator Source = m_fileSource.find(entry.path().extension().string());
		if (Source != m_fileSource.end())Source->second(itemPath);

		//GameObjectの保存
		if (is_directory)MyDragDrop::TargetGameObjectDataSave(itemPath.string());
		EditFile(entry.path());



		ImGui::TextWrapped(filename.c_str());
		ImGui::NextColumn();
	}
	ImGui::Columns();
}

void Prefab::EditFile(const std::filesystem::path& _path)
{
	if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
	{
		SetOpenDirectoryPath(_path);
	}

	if (m_bOpenFileEditor)return;

	if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
	{
		ImGui::OpenPopup(("EditFile" + _path.string()).c_str());
		m_bOpenFileEditor = true;
	}
	if (ImGui::BeginPopup(("EditFile" + _path.string()).c_str()))
	{
		for (auto& Fn : m_rightClickedFileEdit)Fn(_path);
		ImGui::EndPopup();
	}
}
void Prefab::OpenFile(const std::filesystem::path& _path)
{
	if (_path.has_extension())return;
	if (m_openDirectoryPath == _path)return;
	if (!ImGui::MenuItem("OpenFile"))return;
	m_openDirectoryPath = _path;
	m_directoryChanged = true;
}
void Prefab::ShowExplorer(const std::filesystem::path& _path)
{
	if (_path.has_extension())return;
	if (!ImGui::MenuItem("ShowExplorer"))return;
	std::filesystem::path path = std::filesystem::absolute(_path);
	ShellExecute(NULL, L"open", path.c_str(), NULL, NULL, SW_SHOWNORMAL);
}
void Prefab::NewFile(const std::filesystem::path& _path)
{
	if (_path.has_extension())return;
	std::function<void(std::string)> CraeteFile = [&](std::string _name)
		{
			if (ImGui::MenuItem(_name.c_str()))
			{
				std::ofstream file(_path.string() + "/" + _name);
				if (file.is_open()) file.close();
			}
		};
	if (ImGui::BeginMenu("NewFile"))
	{
		CraeteFile("Script.h");
		CraeteFile("Script.cpp");
		ImGui::EndMenu();
	}
}
void Prefab::RenameFile(const std::filesystem::path& _path)
{
	if (!ImGui::MenuItem("RenameFile"))return;
}
void Prefab::RemoveFile(const std::filesystem::path& _path)
{
	if (m_openDirectoryPath == _path)return;
	if (!ImGui::MenuItem("RemoveFile"))return;
	std::filesystem::remove_all(_path);
}
void Prefab::SetOpenDirectoryPath(const std::filesystem::path& _path)
{
	if (ImGui::IsItemToggledOpen())return;
	if (!std::filesystem::is_directory(_path))return;
	m_openDirectoryPath = _path;
	m_directoryChanged = true;
}

void Prefab::ConfigLoadContents(nlohmann::json& _json)
{
	if(_json["OpenDirectoryPath"].is_string())m_openDirectoryPath = _json["OpenDirectoryPath"].get<std::string>();
	for (auto& path : _json["FavoritePaths"])
	{
		m_favoritePathList.push_back(_json);
	}
}

void Prefab::ConfigSaveContents(nlohmann::json& _json)
{
	_json["OpenDirectoryPath"] = m_openDirectoryPath;
	_json["FavoritePaths"] = nlohmann::json::array();
	for (auto& path : m_favoritePathList)
	{
		_json.push_back(path);
	}
}

#include "../SetUpScene/SetUpScene.h"

Prefab::Prefab()
{
	m_fileSource[".prefab"] = [&](const std::filesystem::path& _path) {return MyDragDrop::SourceGameObjectDataPath(_path.string()); };
	m_fileSource[".png"] = [&](const std::filesystem::path& _path) {return MyDragDrop::SourcePicture(_path.string()); };
	m_fileSource[".gltf"] = [&](const std::filesystem::path& _path) {return MyDragDrop::SourceModel(_path.string()); };
	m_fileSource[".scene"] = [&](const std::filesystem::path& _path) {return MyDragDrop::SourceScene(_path.string()); };

	m_leftClickedFileEdit.push_back([&](const std::filesystem::path& _path) {SetOpenDirectoryPath(_path); });

	m_rightClickedFileEdit.push_back([&](const std::filesystem::path& _path) {OpenFile(_path); });
	m_rightClickedFileEdit.push_back([&](const std::filesystem::path& _path) {NewFile(_path); });
	m_rightClickedFileEdit.push_back([&](const std::filesystem::path& _path) {ShowExplorer(_path); });
	m_rightClickedFileEdit.push_back([&](const std::filesystem::path& _path) {RenameFile(_path); });
	m_rightClickedFileEdit.push_back([&](const std::filesystem::path& _path) {RemoveFile(_path); });
}

namespace MyDragDrop
{
	bool SourceGameObjectDataPath(std::string _path)
	{
		return MyImGui::DragDropSource("GameObjectDataPath", _path);
	}
	bool TargetGameObjectDataPath(std::weak_ptr<GameObject> _obj)
	{
		bool flg = false;
		if (std::string path; MyImGui::DragDropTarget("GameObjectDataPath", path))
		{
			GameObjectManager::CreateObject(path, _obj);
			flg = true;
		}
		return flg;
	}

	bool SourceGameObjectData(std::weak_ptr<GameObject> _obj)
	{
		return MyImGui::DragDropSource("GameObject", _obj);
	}
	bool TargetGameObjectData(std::weak_ptr<GameObject> _parent)
	{
		bool flg = false;
		if (std::weak_ptr<GameObject> obj; MyImGui::DragDropTarget("GameObject", obj))
		{
			std::function<void(std::weak_ptr<GameObject>)>Fn = [&](std::weak_ptr<GameObject> _obj)
				{
					if (_obj.expired())
					{
						obj.lock()->SetUpParent(_parent);
						return;
					}
					if (obj.lock() == _obj.lock())return;
					Fn(_obj.lock()->GetParent());
				};
			Fn(_parent);
			flg = false;
		}
		return flg;
	}

	void TargetGameObjectDataSave(std::string _path)
	{
		if (std::weak_ptr<GameObject> obj; MyImGui::DragDropTarget("GameObject", obj))
		{
			MyJson::OutputJson(obj.lock()->OutPutFamilyJson(), (_path.empty() ? "" : _path + "/") + obj.lock()->GetName() + ".prefab");
		}
	}
}

