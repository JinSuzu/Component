#include "SceneActive.h"
#include "../../Game/GameObject.h"
#include "../../../SceneBase/Manager/SceneManager.h"
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
	bool nowFlg = m_sceneName == SceneManager::Instance().GetScenePhase();
	if (Application::Instance().GetBuildFlg())nowFlg = false;

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

void Cp_SceneActive::InitJson()
{
	if (m_jsonData["SceneName"].is_string())m_sceneName = m_jsonData["SceneName"];
	if (m_jsonData["SceneBGM"].is_string())m_sceneBGMPath = m_jsonData["SceneBGM"];
}

nlohmann::json Cp_SceneActive::GetJson()
{
	m_jsonData["SceneName"] = m_sceneName;
	m_jsonData["SceneBGM"] = m_sceneBGMPath;
	return m_jsonData;
}
