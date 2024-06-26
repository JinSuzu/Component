#include "SceneChanger.h"
#include "../../Game/GameObject.h"
#include "../../../SceneBase/Manager/SceneManager.h"

void Cp_SceneChanger::PreUpdateContents()
{
	std::list<std::weak_ptr<GameObject>>* list = m_owner.lock()->WorkChilds();
	std::list<std::weak_ptr<GameObject>>::iterator it = m_owner.lock()->GetChilds().begin();
	while(it != m_owner.lock()->GetChilds().end())
	{
		if (it->expired()) 
		{
			it = m_owner.lock()->WorkChilds()->erase(it);
			continue;
		}
		it++;
	}

	if (m_owner.lock()->GetChilds().empty())
	{
		SceneManager::Instance().SetScenePhase(m_nextSceneName);
		if (!m_sePath.empty())KdAudioManager::Instance().Play("Asset/Sounds/" + m_sePath + ".wav");
	}
}

void Cp_SceneChanger::ImGuiUpdate()
{
	ImGui::InputText("NextScene", &m_nextSceneName);
	ImGui::InputText("SEPath", &m_sePath);
}

void Cp_SceneChanger::InitJson()
{
	if (m_jsonData["NextSceneName"].is_string())m_nextSceneName = m_jsonData["NextSceneName"];
	if (m_jsonData["SEPath"].is_string())m_sePath = m_jsonData["SEPath"];
}

nlohmann::json Cp_SceneChanger::GetJson()
{
	m_jsonData["NextSceneName"] = m_nextSceneName;
	m_jsonData["SEPath"] = m_sePath;
	return m_jsonData;
}
