#pragma once
#include "../Base/EditorWindowBase.h"

class GameObject;

class Prefab
	:public EditorWindowBase
{
public:
	void Update() override;

	static bool SourceGameObject(std::weak_ptr<GameObject> _obj);
	static void TargetGameObject(std::weak_ptr<GameObject> _parent);
private:
	//相対パスの底から全てのディレクトリを追う奴
	void DirectoryTree(const std::filesystem::path& _path);
	//選択されたディレクトの中身見る奴
	void DirectoryContents();
	bool EditFile(const std::filesystem::path& _path);

	void TargetGameObjectSave(std::string _path);
	bool SourceGameObjectDataPath(std::string _path);

	void OpenFile(const std::filesystem::path& _path);
	void NewFile(const std::filesystem::path& _path);
	void RenameFile(const std::filesystem::path& _path);
	void RemoveFile(const std::filesystem::path& _path);

	std::filesystem::path							m_openDirectoryPath;
	bool											m_directoryChanged = false;
	std::list<std::string>							m_favoritePathList;

	std::unordered_map<std::string, std::function<bool(std::string)>> m_fileSource;
	std::list<std::function<void(const std::filesystem::path&)>>	  m_fileEdit;

	void Init();
public:
	Prefab()
	{
		Init();
		m_favoritePathList.push_back(("Asset/Data/PreSet"));
		m_favoritePathList.push_back(("Asset/Data/Model"));
		m_favoritePathList.push_back(("Asset/Textures"));
		m_favoritePathList.push_back(("Asset/Sounds"));
	}
};
