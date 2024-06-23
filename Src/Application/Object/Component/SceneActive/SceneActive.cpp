#include "SceneActive.h"
#include "../../Game/GameObject.h"
#include "../../../SceneBase/Manager/SceneManager.h"

void Cp_SceneActive::Start()
{
	/*bool nowFlg = m_sceneName == SceneManager::Instance().m_nowPhaseName;
	m_flg = nowFlg;
	for (auto& it : m_owner.lock()->GetChilds())
	{
		if (it.expired())continue;
		it.lock()->SetActive(m_flg);
	}*/
	m_flg = false;
}

void Cp_SceneActive::PreUpdateContents()
{
	bool nowFlg = m_sceneName == SceneManager::Instance().GetScenePhase();
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
}

void Cp_SceneActive::InitJson()
{
	if (m_jsonData["SceneName"].is_string())m_sceneName = m_jsonData["SceneName"];
}

nlohmann::json Cp_SceneActive::GetJson()
{
	m_jsonData["SceneName"] = m_sceneName;
	return m_jsonData;
}
