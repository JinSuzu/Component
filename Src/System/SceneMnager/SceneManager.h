#pragma once

class SceneManager
{
public:
	void LoadScene(std::string _name);

	const std::string& GetNowSceneName()			const { return m_nowScene; };
	std::map <std::string, std::string>& WorkSceneList() { return m_sceneList; };

	void PushScene();

	void LoadWait() const;

	void SaveScene();

	void Init();
	void Release();
private:
	void Loading();
	std::map <std::string, std::string> m_sceneList;

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
