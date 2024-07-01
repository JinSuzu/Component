#pragma once

class GameObject;
class Editor
{
public:
	void ImGuiUpdate();

	void MenuBar();
	void Prefab(    ImVec2 _size,UINT _flg = ImGuiChildFlags_FrameStyle);
	void Inspector( ImVec2 _size,UINT _flg = ImGuiChildFlags_FrameStyle);
	void Hierarchy( ImVec2 _size,UINT _flg = ImGuiChildFlags_FrameStyle);
	void GameScreen(ImVec2 _size,UINT _flg = ImGuiChildFlags_FrameStyle);

	void SetEditObject(std::weak_ptr<GameObject> _obj) { m_editObject = _obj; }
private:

	void DirectoryTree();
	void DirectoryTree(std::string _name, std::string _path);
	void DirectoryContents();

	std::string m_openDirectoryPath;
	bool m_directoryChanged = false;


	void ShowRowTabGroup(int _row);

	std::weak_ptr<GameObject> m_editObject;
	std::map<std::string, std::function<void()>>m_editorWindows;


	struct TabGroup
	{
		std::list<std::string> contents;
	};
	std::vector<std::vector<TabGroup>>m_windows;
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
		/*std::function<void(std::string, std::function<void()>)> Register = [&](std::string _tag, std::function<void()> _function)
			{
				m_editorWindows[_tag] = _function;
			};

		Register("Prefab", [&]() {Prefab(ImVec2()); });
		Register("Inspector", [&]() {Inspector(); });
		Register("Hierarchy", [&]() {Hierarchy(); });
		Register("GameScreen", [&]() {GameScreen(); });

		TabGroup tabGroup;
		std::vector<TabGroup> OneLine;
		tabGroup.contents.push_back("Hierarchy");
		OneLine.push_back(tabGroup);
		tabGroup = TabGroup();
		tabGroup.contents.push_back("GameScreen");
		OneLine.push_back(tabGroup);
		tabGroup = TabGroup();
		tabGroup.contents.push_back("Inspector");
		OneLine.push_back(tabGroup);
		m_windows.push_back(OneLine);

		std::vector<TabGroup> twoLine;
		tabGroup = TabGroup();
		tabGroup.contents.push_back("Prefab");
		twoLine.push_back(tabGroup);
		m_windows.push_back(twoLine);*/
	}
};

