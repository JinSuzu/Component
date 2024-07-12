#pragma once

class SceneManager
{
public:
	void LoadScene(std::string _name);

	const std::string& GetNowSceneName() const { return m_nowScene; };

	void ImGuiUpdate();
	void PushScene();

	void Loading();
	void LoadWait() const;

	void SaveScene();

	void Init();
private:
	std::unordered_map <std::string, std::string> m_sceneList;

	bool										  m_callLoad = false;
	std::thread									  m_thread;

	std::list<std::shared_ptr<class GameObject>>  m_loadedGameObjectList;

	SceneManager() :m_thread([&]() {Loading(); }) {}
	~SceneManager() { m_thread.join(); }
public:
	std::string m_nowScene;
	static SceneManager& Instance()
	{
		static SceneManager inst;
		return inst;
	}
};
