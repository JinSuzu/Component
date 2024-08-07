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
	bool EditFile				(const std::filesystem::path& _path);
	

	void ConfigLoadContents(nlohmann::json& _json)override;
	void ConfigSaveContents(nlohmann::json& _json)override;

	bool															  m_bOpenFileEditor = false;
	std::list<std::function<void(const std::filesystem::path&,bool&)>>m_rightClickedFileEdit;
	
	std::filesystem::path											  m_openDirectoryPath;
	bool															  m_directoryChanged = false;
	std::list<std::string>											  m_favoritePathList;

	std::unordered_map<std::string, std::function<bool(const std::filesystem::path&)>> m_fileSource;
public:
	void Init()override;
	void Release()override;
};

namespace MyDragDrop
{
constexpr const char* GAMEOBJECT_PATH_ID = "GAMEOBJECT_PATH_ID";
	bool SourceGameObjectDataPath(std::string _path);
	bool TargetGameObjectDataPath(std::weak_ptr<GameObject> _obj);
	bool TargetGameObjectDataPath(std::string& _obj);

	bool SourceGameObjectData(std::weak_ptr<GameObject> _obj);
	bool TargetGameObjectData(std::weak_ptr<GameObject> _parent);
	void TargetGameObjectDataSave(std::string _path);

	/*
	bool SourceDirectoryPath(std::string _path);
	bool TargetRegisterFavorite(std::list<std::string>& list);
	*/
}