#include "SceneActiveComponent.h"
#include "../../../Game/GameObject.h"
#include "../../../../Utility/Timer.h"

#include "../../../../main.h"

SceneActiveComponent::~SceneActiveComponent()
{
	if (m_sceneBGM)
	{
		m_sceneBGM->Stop();
	}
}

void SceneActiveComponent::Start()
{
	/*bool nowFlg = m_sceneName == SceneManager::Instance().m_nowPhaseName;
	m_flg = nowFlg;
	for (auto& it : m_owner.lock()->GetChilds())
	{
		if (it.expired())continue;
		it.lock()->SetActive(m_flg);
	}*/
	m_flg = true;
}

void SceneActiveComponent::PreUpdateContents()
{
	bool nowFlg = m_sceneName == SceneManager::Instance().GetNowSceneName();
	if (KernelEngine::is_Building())nowFlg = false;

	if (m_flg)
	{
		if (!m_sceneBGM && !m_sceneBGMPath.empty())
		{
			m_sceneBGM = KdAudioManager::Instance().Play("Asset/Sounds/" + m_sceneBGMPath + ".wav", true);
			Timer::Instance().Start(m_sceneName);
		}
	}
	else if (m_sceneBGM)
	{
		m_sceneBGM->Stop();
		m_sceneBGM = nullptr;
		Timer::Instance().Stop(m_sceneName);
	}

	if (m_flg == nowFlg)return;

	m_flg = nowFlg;
	for (auto& it : m_owner.lock()->GetChilds())
	{
		if (it.expired())continue;
		it.lock()->SetActive(m_flg);
	}
}

void SceneActiveComponent::ImGuiUpdate()
{
	ImGui::InputText("SceneName", &m_sceneName);
	ImGui::InputText("SceneBGM", &m_sceneBGMPath);
}

void SceneActiveComponent::LoadJson(nlohmann::json _json)
{
	if (_json["SceneName"].is_string())m_sceneName   = _json["SceneName"];
	if (_json["SceneBGM"].is_string())m_sceneBGMPath = _json["SceneBGM"];
}

nlohmann::json SceneActiveComponent::GetJson()
{
	nlohmann::json json;
	json["SceneName"] = m_sceneName;
	json["SceneBGM"] = m_sceneBGMPath;
	return json;
}
