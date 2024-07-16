#pragma once
#include "../Base/EditorWindowBase.h"

class GameObject;

class Prefab
	:public EditorWindowBase
{
public:
	void UpdateContents() override;
private:
	//相対パスの底から全てのディレクトリを追う奴
	void DirectoryTree(const std::filesystem::path& _path);
	//選択されたディレクトの中身見る奴
	void DirectoryContents();

	//ファイルをいじる用の関数
	void EditFile(const std::filesystem::path& _path);
	void OpenFile(const std::filesystem::path& _path);
	void ShowExplorer(const std::filesystem::path& _path);
	void NewFile(const std::filesystem::path& _path);
	void RenameFile(const std::filesystem::path& _path);
	void RemoveFile(const std::filesystem::path& _path);
	void SetOpenDirectoryPath(const std::filesystem::path& _path);

	void ConfigLoadContents(nlohmann::json& _json)override;
	void ConfigSaveContents(nlohmann::json& _json)override;

	bool															  m_bOpenFileEditor = false;
	std::list<std::function<void(const std::filesystem::path&)>>	  m_leftClickedFileEdit;
	std::list<std::function<void(const std::filesystem::path&)>>	  m_rightClickedFileEdit;
	
	std::filesystem::path											  m_openDirectoryPath;
	bool															  m_directoryChanged = false;
	std::list<std::string>											  m_favoritePathList;

	std::unordered_map<std::string, std::function<bool(const std::filesystem::path&)>> m_fileSource;
public:
	Prefab();
};

namespace MyDragDrop
{
	bool SourceGameObjectDataPath(std::string _path);
	bool TargetGameObjectDataPath(std::weak_ptr<GameObject> _obj);

	bool SourceGameObjectData(std::weak_ptr<GameObject> _obj);
	bool TargetGameObjectData(std::weak_ptr<GameObject> _parent);
	void TargetGameObjectDataSave(std::string _path);
}