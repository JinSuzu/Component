#pragma once
#include "../Base/EditorWindowBase.h"

class Prefab
	:public EditorWindowBase
{
public:
	void Update() override;
private:
	void DirectoryTree();
	void DirectoryTree(std::string _name, std::string _path);
	void DirectoryContents();

	void TargetGameObjectSave(std::string _path);
	bool SourceGameObjectDataPath(std::string _path);

	std::string										m_openDirectoryPath;
	bool											m_directoryChanged = false;
	std::list<std::pair<std::string, std::string>>	m_favoritePathList;
public:
	Prefab()
	{
		m_favoritePathList.push_back(std::pair<std::string, std::string>("Asset/Data/", "PreSet"));
		m_favoritePathList.push_back(std::pair<std::string, std::string>("Asset/Data/", "Model"));
		m_favoritePathList.push_back(std::pair<std::string, std::string>("Asset/", "Textures"));
		m_favoritePathList.push_back(std::pair<std::string, std::string>("Asset/", "Sounds"));
	}
};
