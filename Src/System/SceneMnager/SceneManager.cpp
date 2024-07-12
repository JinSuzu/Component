#include "SceneManager.h"
#include "../../Application/main.h"
#include "../../Application/ImGuiHelper/ImGuiHelper.h"
#include "../../Application/JsonHelper/JsonHelper.h"
#include "../../Application/Object/Game/GameObject.h"
#include "../../Application/Object/Game/Manager/GameObjectManager.h"

void SceneManager::LoadScene(std::string _name)
{
	std::unordered_map<std::string, std::string>::iterator it = m_sceneList.find(_name);
	if (it == m_sceneList.end())
	{
		Application::Instance().m_log.AddLog("No Found Scene!!");
		return;
	}

	m_callLoad = true;
	m_nowScene = _name;
}

void SceneManager::ImGuiUpdate()
{
	ImGui::BeginChild("SceneList", ImGui::GetContentRegionAvail(), ImGuiChildFlags_Border);
	{
		for (auto& map : m_sceneList)
		{
			std::string name = map.first;
			if (MyImGui::InputText("", name))
			{
				m_sceneList.erase(name);
				m_sceneList[name] = map.first;
				break;
			}

			ImGui::SameLine();

			std::string path = map.second;
			if (MyImGui::InputText("", path))map.second = path;

			ImGui::SameLine();

			if (ImGui::Button("Remove")) 
			{
				m_sceneList.erase(map.first);
				break;
			}
		}
	}
	ImGui::EndChild();
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
			nlohmann::json json = MyJson::InPutJson(m_sceneList[m_nowScene]);
			for (auto& it : json) 
			{
				GameObjectManager::CreateObject((it),std::weak_ptr<GameObject>(),false, &m_loadedGameObjectList);
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
	GameObjectManager::Instance().Release(m_sceneList[m_nowScene],Application::Instance().GetBuildFlg());
	LoadScene(m_nowScene);
}

void SceneManager::Init()
{
	m_sceneList["Title"] = "title.scene";
	m_nowScene = "Title";
	LoadScene(m_nowScene);
}
