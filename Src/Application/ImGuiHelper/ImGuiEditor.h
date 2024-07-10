#pragma once

class GameObject;
class Editor
{
public:
	void ImGuiUpdate();

	void MenuBar();

	void SetEditObject(std::weak_ptr<GameObject> _obj) { m_editObject = _obj; }
	std::weak_ptr<GameObject> GetEditObject() { return m_editObject; }

private:
	std::weak_ptr<GameObject>														m_editObject;
	std::map<std::string, std::function<std::shared_ptr<class EditorWindowBase>()>>	m_editorWindows;

	std::map<std::string, bool>														m_editorActive;
	std::list<std::shared_ptr<class EditorWindowBase>>								m_windowList;
	void OverwriteWindow();

	void Init();
	void Release();

public:
	Editor() { Init(); };
	~Editor() { Release(); };
};

