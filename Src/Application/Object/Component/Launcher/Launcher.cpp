#include "Launcher.h"
#include "../../Game/GameObject.h"
#include "../../Game/Manager/GameObjectManager.h"
#include "../ModelData/ModelData.h"
#include "../Transform/Transform.h"
#include "../../../AssetManager/AssetManager.h"

void Cp_Launcher::Start()
{
	m_launcherModel[State::UnLoad] = AssetManager::Instance().GetModelData(m_unLoadModelPath);
	m_launcherModel[State::Loaded] = AssetManager::Instance().GetModelData(m_loadedModelPath);

	m_draw = m_owner.lock()->GetComponent<Cp_ModelData>();
	assert(m_draw.lock() && "modeldataコンポーネントが見かりませんでした by Launcher");

	m_bullet = GameObjectManager::CreateObject(m_bulletPath, m_owner, false);
	m_bullet->SetActive(false);
	m_bullet->SetHideFlg(true);
	m_bullet->DotSave();
}

void Cp_Launcher::PreUpdateContents()
{
	if (m_draw.expired())return;
	if (m_bulletNum > 0)m_state = State::Loaded;
	else m_state = State::UnLoad;

	if (m_state == State::Loaded)
	{
		if (m_bShotStandby)
		{
			if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
			{
				GameObjectManager::CreateObject(m_bulletPath, m_owner)->GetTransform();
				m_bulletNum--;

				m_bShotStandby = false;
				m_shotIntervalCnt = m_shotInterval;
			}
		}
	}

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
}

void Cp_Launcher::ImGuiUpdate()
{
	ImGui::InputInt("BulletMax", &m_bulletMax);
	ImGui::InputInt("ShotInterval", &m_shotInterval);
	ImGui::InputInt("ReLoadTime", &m_reloadTime);

	static bool edit = false;
	if (ImGui::Button("BulletEdit"))ImGui::OpenPopup(("BulletEdit##" + std::to_string(m_instanceID)).c_str());
	if (ImGui::BeginPopup(("BulletEdit##" + std::to_string(m_instanceID)).c_str())) 
	{
		edit = true;
		GameObjectManager::ImGuiGameObject(m_bullet);
		ImGui::EndPopup();
	}
	else if(edit) 
	{
		m_bullet->EnableSave();
		m_bullet->Release();
		m_bullet->DotSave();
		edit = false;
	}

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

	ImGui::InputText("BulletPath", &m_bulletPath);
	if (ImGui::SameLine(); ImGui::Button("Input"))
	{
		m_bullet = GameObjectManager::CreateObject(m_bulletPath, m_owner,false);
		m_bullet->SetActive(false);
		m_bullet->SetHideFlg(true);
		m_bullet->DotSave();
	}
}

void Cp_Launcher::InitJson()
{
	m_loadedModelPath = m_jsonData["loadedModelPath"];
	m_unLoadModelPath = m_jsonData["unLoadModelPath"];
	m_bulletPath = m_jsonData["bulletPath"];

	m_bulletMax = m_bulletNum = m_jsonData["bulletMax"];
	m_shotInterval = m_jsonData["shotInterval"];
	m_reloadTime = m_jsonData["reloadTime"];

	m_bullet = GameObjectManager::CreateObject(m_bulletPath, m_owner, false);
	m_bullet->SetActive(false);
	m_bullet->SetHideFlg(true);
	m_bullet->DotSave();
}

nlohmann::json Cp_Launcher::GetJson()
{

	m_jsonData["loadedModelPath"] = m_loadedModelPath;
	m_jsonData["unLoadModelPath"] = m_unLoadModelPath;
	m_jsonData["bulletPath"] = m_bulletPath;

	m_jsonData["bulletMax"] = m_bulletMax;
	m_jsonData["shotInterval"] = m_shotInterval;
	m_jsonData["reloadTime"] = m_reloadTime;
	return m_jsonData;
}

