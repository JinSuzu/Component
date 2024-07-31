#include "Launcher.h"
#include "../../Game/GameObject.h"
#include "../ModelData/ModelData.h"
#include "../Transform/Transform.h"

#include "../../../Utility/CreateObject/CreateObject.h"

void Cp_Launcher::Start()
{
	m_launcherModel[State::UnLoad] = AssetManager::Instance().GetModelData(m_unLoadModelPath);
	m_launcherModel[State::Loaded] = AssetManager::Instance().GetModelData(m_loadedModelPath);

	m_draw = m_owner.lock()->GetComponent<Cp_ModelData>();
	assert(m_draw.lock() && "modeldataコンポーネントが見かりませんでした by Launcher");

	m_bullet = std::make_shared<CreateObject>(m_owner);
}

void Cp_Launcher::PreUpdateContents()
{
	if (m_draw.expired())return;
	if (m_bulletNum > 0)m_state = State::Loaded;
	else m_state = State::UnLoad;

	if (m_shotIntervalCnt <= 0)
	{
		m_bShotStandby = true;
	}
	else m_shotIntervalCnt--;

	if (m_state == State::UnLoad)
	{

		if (m_reloadTimeCnt-- <= 0 && m_bReloading)
		{
			m_bulletNum = m_bulletMax;
			m_bReloading = false;
		}

		else if (GetAsyncKeyState('R') & 0x8000 && !m_bReloading)
		{
			m_reloadTimeCnt = m_reloadTime;
			m_bReloading = true;
		}
	}

	m_draw.lock()->SetModelData(m_launcherModel[m_state]);

	if (m_state == State::Loaded)
	{
		if (m_bShotStandby)
		{
			if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
			{
				KdAudioManager::Instance().Play("Asset/Sounds/Shot.wav");
				m_bullet->Create();
				m_bulletNum--;

				m_bShotStandby = false;
				m_shotIntervalCnt = m_shotInterval;
			}
		}
	}

}

void Cp_Launcher::ImGuiUpdate()
{
	ImGui::InputInt("BulletMax", &m_bulletMax);
	ImGui::InputInt("ShotInterval", &m_shotInterval);
	ImGui::InputInt("ReLoadTime", &m_reloadTime);

	m_bullet->ImGuiUpdate();

	ImGui::InputText("LoadedModelPath", &m_loadedModelPath);
	if (ImGui::SameLine(); ImGui::Button("Input"))
	{
		m_launcherModel[State::Loaded] = AssetManager::Instance().GetModelData(m_loadedModelPath);
	}

	ImGui::InputText("UnLoadModelPath", &m_unLoadModelPath);
	if (ImGui::SameLine(); ImGui::Button("Input"))
	{
		m_launcherModel[State::UnLoad] = AssetManager::Instance().GetModelData(m_unLoadModelPath);
	}
}

void Cp_Launcher::LoadJson(nlohmann::json _json)
{
	m_loadedModelPath = _json["loadedModelPath"];
	m_unLoadModelPath = _json["unLoadModelPath"];

	m_bullet->SetJson(_json);

	m_bulletMax = m_bulletNum = _json["bulletMax"];
	m_shotInterval = _json["shotInterval"];
	m_reloadTime   = _json["reloadTime"];
}

nlohmann::json Cp_Launcher::GetJson()
{
	nlohmann::json json;
	json["loadedModelPath"] = m_loadedModelPath;
	json["unLoadModelPath"] = m_unLoadModelPath;
	m_bullet->OutJson(json);

	json["bulletMax"] = m_bulletMax;
	json["shotInterval"] = m_shotInterval;
	json["reloadTime"] = m_reloadTime;
	return json;
}