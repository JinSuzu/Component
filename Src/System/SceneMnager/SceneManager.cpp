#include "SceneManager.h"
#include "../../Application/main.h"
#include "../../Application/ImGuiHelper/ImGuiHelper.h"
#include "../../Application/JsonHelper/JsonHelper.h"
#include "../../Application/Object/Game/GameObject.h"
#include "../../Application/Object/Game/Manager/GameObjectManager.h"

void SceneManager::LoadScene(std::string _name)
{
	std::map<std::string, std::string>::iterator it = m_sceneList.find(_name);
	if (it == m_sceneList.end())
	{
		Application::Instance().m_log.AddLog("No Found Scene!!");
		return;
	}
	if (!std::filesystem::exists((*it).second))
	{
		Application::Instance().m_log.AddLog("Scene FilePath Broken!!\n");
		return;
	}
	m_callLoad = true;
	m_nowScene = _name;
}

void SceneManager::PushScene()
{
	LoadWait();
	if (!m_loadedGameObjectList.empty())
	{
		GameObjectManager::Instance().Load(m_loadedGameObjectList);
		m_loadedGameObjectList.clear();
	}
}

void SceneManager::Loading()
{
	Application::Instance().m_log.AddLog("Let's GO!!!!!");
	while (Application::Instance().GetWindow().IsCreated())
	{
		if (m_callLoad)
		{
			m_loadedGameObjectList.clear();
			nlohmann::json json = MyJson::InputJson(m_sceneList[m_nowScene]);
			for (auto& it : json)
			{
				GameObjectManager::CreateObject((it), std::weak_ptr<GameObject>(), false, &m_loadedGameObjectList);
			}
			m_callLoad = false;
		}
	}
}
void SceneManager::LoadWait() const
{
	if (!m_callLoad)return;
	std::thread wait([&]() {while (m_callLoad); });
	wait.join();
}

void SceneManager::SaveScene()
{
	GameObjectManager::Instance().Release(m_sceneList[m_nowScene], Application::Instance().GetBuildFlg());
	LoadScene(m_nowScene);
}

void SceneManager::Init()
{
	ConfigManger config;
	nlohmann::json data;
	if (config.Load("Scene", data))
	{
		m_nowScene = data["NowScene"];
		nlohmann::json::iterator map = data["SceneData"].begin();
		while (map != data["SceneData"].end())
		{
			m_sceneList.emplace(map.key(), *map);
			map++;
		}
	}
	else
	{
		m_nowScene = "TitleA";
		m_sceneList["TitleC"] = "pasth.scene";
		m_sceneList["TitleA"] = "pasth.scene";
		m_sceneList["TitleB"] = "pasth.scene";
	}
	LoadScene(m_nowScene);
}

void SceneManager::Release()
{
	nlohmann::json data;
	data["NowScene"] = m_nowScene;
	data["SceneData"] = nlohmann::json::object();
	for (auto& map : m_sceneList)
	{
		data["SceneData"][map.first] = map.second;
	}
	ConfigManger config;
	config.Save("Scene", data);
}
