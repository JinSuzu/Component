#pragma once

class SceneBase;
enum SceneID
{
	Title,
	Game,
	Result,
	Kurosaki,
	Yamamoto,
	Motoori,
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

	void DrawLoad(bool& flg);

	int GetNowSceneNum() const { return m_nowSceneNum; }
	std::weak_ptr<SceneBase> GetNowScene() { return m_scene; }
private:

	std::shared_ptr<SceneBase> m_scene;
	std::map<SceneID,std::function<std::shared_ptr<SceneBase>()>> m_registerScene;
	SceneID m_nowSceneNum;

	SceneManager()
		:m_scene()
		,m_registerScene()
		,m_nowSceneNum()
	{}
public:
	static SceneManager& Instance()
	{
		static SceneManager instance;
		return instance;
	}

	void ReLoad();
};

