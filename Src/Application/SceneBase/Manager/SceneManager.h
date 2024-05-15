#pragma once

class SceneBase;
enum SceneID;

class SceneManager
{
public:
	~SceneManager() {}

	void Draw();

	void PreUpdate();
	void Update();
	void PostUpdate();

	void Init();
	void ImGuiUpdate();

	void ShiftScene(SceneID a_toSceneNum);

	void DrawLoad(bool& flg);

	int GetNowSceneNum() { return m_nowSceneNum;}
private:

	std::shared_ptr<SceneBase> m_scene;
	std::vector<std::function<std::shared_ptr<SceneBase>()>> m_geneSceneList;
	SceneID m_nowSceneNum;

	SceneManager()
		:m_scene()
		,m_geneSceneList()
		,m_nowSceneNum()
	{}
public:
	static SceneManager& Instance()
	{
		static SceneManager instance;
		return instance;
	}
};

