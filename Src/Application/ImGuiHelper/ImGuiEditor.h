#pragma once

class GameObject;
class Editor
{
public:
	void ImGuiUpdate();

	void MenuBar();
	void Prefab();
	void Inspector();
	void Hierarchy();
	void GameScreen();

	void SetEditObject(std::weak_ptr<GameObject> _obj) { m_editObject = _obj; }
private:

	void DirectoryTree();
	void DirectoryTree(std::string _name, std::string _path);
	void DirectoryContents();

	std::string										m_openDirectoryPath;
	bool											m_directoryChanged = false;
	std::list<std::pair<std::string, std::string>>	m_favoritePathList;


	std::weak_ptr<GameObject>					m_editObject;
	std::map<std::string, std::function<void()>>m_editorWindows;
	std::list<std::string>						m_tabList;

public:
	static bool SourceGameObject(std::weak_ptr<GameObject> _obj);
	static void TargetGameObject(std::weak_ptr<GameObject> _parent);
	static void TargetGameObjectSave(std::string _path);

	static bool SourceGameObjectDataPath(std::string _path);

	void CheckMouseApproaching() {
		ImVec2 mouse_pos = ImGui::GetMousePos();
		ImVec2 window_pos = ImGui::GetWindowPos();
		ImVec2 window_size = ImGui::GetWindowSize();

		float left_threshold = window_pos.x + window_size.x * 0.1f;
		float right_threshold = window_pos.x + window_size.x * 0.9f;
		float top_threshold = window_pos.y + window_size.y * 0.1f;
		float bottom_threshold = window_pos.y + window_size.y * 0.9f;

		std::string mouse_position = "Center";

		if (mouse_pos.x < left_threshold) {
			mouse_position = "Left";
		}
		else if (mouse_pos.x > right_threshold) {
			mouse_position = "Right";
		}

		if (mouse_pos.y < top_threshold) {
			mouse_position = "Top";
		}
		else if (mouse_pos.y > bottom_threshold) {
			mouse_position = "Bottom";
		}
	}

	//Render
	Editor()
	{
		std::function<void(std::string, std::function<void()>)> Register = [&](std::string _tag, std::function<void()> _function)
			{
				m_editorWindows[_tag] = _function;
			};

		Register("Prefab", [&]() {Prefab(); });
		Register("Inspector", [&]() {Inspector(); });
		Register("Hierarchy", [&]() {Hierarchy(); });
		Register("GameScreen", [&]() {GameScreen(); });

		m_tabList.push_back("Prefab");
		m_tabList.push_back("Inspector");
		m_tabList.push_back("Hierarchy");
		m_tabList.push_back("GameScreen");

		m_favoritePathList.push_back(std::pair<std::string,std::string>("Asset/Data/","PreSet"));
		m_favoritePathList.push_back(std::pair<std::string,std::string>("Asset/Data/","Model"));
		m_favoritePathList.push_back(std::pair<std::string,std::string>("Asset/","Textures"));
		m_favoritePathList.push_back(std::pair<std::string,std::string>("Asset/","Sounds"));
	}
};

