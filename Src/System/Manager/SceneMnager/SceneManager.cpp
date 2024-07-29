#include "SceneManager.h"
#include "../../../Application/main.h"
#include "../../../Application/Object/Game/GameObject.h"
#include "../../../Application/Object/Game/Manager/GameObjectManager.h"

#include "../../../System/EditorWindow/DebugLog/DebugLog.h"

void SceneManager::LoadScene(std::string _name)
{
	std::map<std::string, std::string>::iterator it = m_sceneList.find(_name);
	if (it == m_sceneList.end())
	{
		Application:: Instance().AddLog("No Found Scene!!\n");
		return;
	}
	if (!std::filesystem::exists((*it).second))
	{
		Application::Instance().AddLog("Scene FilePath Broken!!\n");
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
	Application::Instance().AddLog("Start Scene Loading Thread\n");
	while (Application::Instance().GetWindow().IsCreated())
	{
		if (m_callLoad)
		{
			m_loadedGameObjectList.clear();
			nlohmann::json json = Utility::JsonHelper::InputJson(m_sceneList[m_nowScene]);
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

void SceneManager::OpenScene()
{

}

void SceneManager::SaveAsScene()
{
	std::string path;
	if (!Application::Instance().GetWindow().SaveFileDialog(path, "ファイルを保存", "*.scene", ".scene"))return;
	

}
void SceneManager::SaveScene()
{
	GameObjectManager::Instance().Release(m_sceneList[m_nowScene], Application::Instance().GetBuildFlg());
	LoadScene(m_nowScene);
}

void SceneManager::Init()
{
	ConfigManger config;
	if (config.Load("Scene"))
	{
		m_nowScene = config["NowScene"];
		nlohmann::json::iterator map = config["SceneData"].begin();
		while (map != config["SceneData"].end())
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
	ConfigManger config;
	config["NowScene"] = m_nowScene;
	config["SceneData"] = nlohmann::json::object();
	for (auto& map : m_sceneList)
	{
		config["SceneData"][map.first] = map.second;
	}
	config.Save("Scene");
}
