#include "SceneActive.h"
#include "../../Game/GameObject.h"
#include "../../../Utility/Timer.h"

#include "../../../main.h"

Cp_SceneActive::~Cp_SceneActive()
{
	if (m_sceneBGM)
	{
		m_sceneBGM->Stop();
	}
}

void Cp_SceneActive::Start()
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

void Cp_SceneActive::PreUpdateContents()
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

void Cp_SceneActive::ImGuiUpdate()
{
	ImGui::InputText("SceneName", &m_sceneName);
	ImGui::InputText("SceneBGM", &m_sceneBGMPath);
}

void Cp_SceneActive::LoadJson(nlohmann::json _json)
{
	if (_json["SceneName"].is_string())m_sceneName   = _json["SceneName"];
	if (_json["SceneBGM"].is_string())m_sceneBGMPath = _json["SceneBGM"];
}

nlohmann::json Cp_SceneActive::GetJson()
{
	nlohmann::json json;
	json["SceneName"] = m_sceneName;
	json["SceneBGM"] = m_sceneBGMPath;
	return json;
}
