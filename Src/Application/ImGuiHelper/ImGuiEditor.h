#pragma once

class GameObject;
class Editor 
{
public:
	void ImGuiUpdate();

	
private:
	void DirectoryTree();
	void DirectoryTree(std::string _name, std::string _path);
	void DirectoryContents();

	std::string m_openDirectoryPath;
	bool m_directoryChanged;

public:
	static bool SourceGameObject(std::weak_ptr<GameObject> _obj);
	static void TargetGameObject(std::weak_ptr<GameObject> _parent);
	static void TargetGameObjectSave(std::string _path);

	static bool SourceGameObjectDataPath(std::string _path);
};

