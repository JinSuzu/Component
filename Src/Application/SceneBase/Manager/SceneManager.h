#pragma once

class SceneBase;
enum SceneID
{
	Scene,
	PreSet,
	Max
};

class SceneManager
{
public:
	~SceneManager() {}

	void PreUpdate();
	void Update();
	void PostUpdate();

	void Init();
	void ImGuiUpdate();

	void ShiftScene(SceneID _toSceneNum);

	int GetNowSceneNum() const { return m_nowSceneNum; }
	std::shared_ptr<class GameObjectManager> m_objectMgr;
	std::string m_nowPhaseName;
private:

	SceneID m_nowSceneNum;

	SceneManager()
		:m_nowSceneNum()
	{}
public:
	static SceneManager& Instance()
	{
		static SceneManager instance;
		return instance;
	}

	void ReLoad();
};

