#pragma once

class GameObject;
class EditorWindowBase;
class DebugLog;

class Editor
{
public:
	void ImGuiDraw();
	void ImGuiUpdate();

	void SetEditObject(std::weak_ptr<GameObject> _obj)	{ m_editObject = _obj; }
	std::weak_ptr<GameObject> GetEditObject()			{ return m_editObject; }

	std::weak_ptr<DebugLog> GetDebugLog()				{ return m_debugLog; }

	void Init();
	void Release();
private:
	void MenuBar();
	void PlayButton();
	void OverwriteWindow();

	std::weak_ptr<GameObject>														m_editObject;
	std::map<std::string, std::function<std::shared_ptr<EditorWindowBase>()>>	m_editorWindows;

	std::map<std::string, bool>														m_editorActive;
	std::list<std::shared_ptr<EditorWindowBase>>									m_windowList;
	std::shared_ptr<DebugLog>														m_debugLog;

	void ReleaseWindows();

	Editor()	{}
	~Editor()	{}
public:
	static Editor& Instance() 
	{
		static Editor inst;
		return inst;
	}
};

