#include "SceneChangerComponent.h"
#include "../../../Game/GameObject.h"

void SceneChangerComponent::PreUpdateContents()
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
		SceneManager::Instance().LoadScene(m_nextSceneName);
		if (!m_sePath.empty())KdAudioManager::Instance().Play("Asset/Sounds/" + m_sePath + ".wav");
	}
}

void SceneChangerComponent::ImGuiUpdate()
{
	ImGui::InputText("NextScene", &m_nextSceneName);
	ImGui::InputText("SEPath", &m_sePath);
}

void SceneChangerComponent::LoadJson(nlohmann::json _json)
{
	if (_json["NextSceneName"].is_string())m_nextSceneName = _json["NextSceneName"];
	if (_json["SEPath"].is_string())m_sePath = _json["SEPath"];
}

nlohmann::json SceneChangerComponent::GetJson()
{
	nlohmann::json json;
	json["NextSceneName"] = m_nextSceneName;
	json["SEPath"] = m_sePath;
	return json;
}
