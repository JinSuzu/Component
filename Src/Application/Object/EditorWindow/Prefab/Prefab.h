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
	

	void TargetGameObjectSave(std::string _path);
	bool SourceGameObjectDataPath(std::string _path);

	bool															  m_bOpenFileEditor = false;
	std::list<std::function<void(const std::filesystem::path&)>>	  m_leftClickedFileEdit;
	std::list<std::function<void(const std::filesystem::path&)>>	  m_rightClickedFileEdit;
	
	std::filesystem::path											  m_openDirectoryPath;
	bool															  m_directoryChanged = false;
	std::list<std::string>											  m_favoritePathList;

	std::unordered_map<std::string, std::function<bool(std::string)>> m_fileSource;
public:
	Prefab();
	//ヒエラルキーからGameObjectを引っ張ってくる
	static bool SourceGameObject(std::weak_ptr<GameObject> _obj);
	static void TargetGameObject(std::weak_ptr<GameObject> _parent);

	void SetOpenDirectoryPath(const std::filesystem::path& _path);

};
