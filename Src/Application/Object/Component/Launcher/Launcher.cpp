#include "Launcher.h"
#include "../../Game/GameObject.h"
#include "../../Game/Manager/GameObjectManager.h"
#include "../ModelData/ModelData.h"
#include "../Transform/Transform.h"
#include "../../../AssetManager/AssetManager.h"

void Cp_Launcher::Start()
{
	m_launcherModel[State::UnLoad]  = AssetManager::Instance().GetModelData(m_unLoadModelPath);
	m_launcherModel[State::Loaded]  = AssetManager::Instance().GetModelData(m_loadedModelPath);

	m_draw = m_owner.lock()->GetComponent<Cp_ModelData>();
	assert(m_draw.lock() && "modeldataコンポーネントが見かりませんでした by Launcher");

	//m_bullet = GameObjectManager::CreateObject(m_bulletPath, false);
}

void Cp_Launcher::PreUpdateContents()
{
	if (m_draw.expired())return;
	if (m_capacity > 0)m_state = State::Loaded;
	else m_state = State::UnLoad;

	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000 && m_state == State::Loaded)
	{
		std::weak_ptr<Cp_Transform> trans = GameObjectManager::CreateObject(m_bulletPath)->GetTransform();
		Math::Vector3 pos = (Math::Matrix::CreateTranslation(0, 0, 0.43) * m_owner.lock()->GetTransform().lock()->GetMatrix()).Translation();
		trans.lock()->SetPosition(pos);
		Math::Vector3 deg = m_owner.lock()->GetParent().lock()->GetTransform().lock()->GetRotation();
		trans.lock()->SetRotation(deg);

		m_capacity--;
	}

	if (GetAsyncKeyState('R') & 0x8000 && m_state == State::UnLoad)
	{
		//後でmaxキャパもつくる
		m_capacity = 1;
	}

	m_draw.lock()->SetModelData(m_launcherModel[m_state]);
}

void Cp_Launcher::ImGuiUpdate()
{
	ImGui::InputText("LoadedModelPath", &m_loadedModelPath);
	if (ImGui::SameLine(); ImGui::Button("Input"))
	{
		m_launcherModel[true] = AssetManager::Instance().GetModelData(m_loadedModelPath);
	}
	
	ImGui::InputText("UnLoadModelPath", &m_unLoadModelPath);
	if (ImGui::SameLine(); ImGui::Button("Input"))
	{
		m_launcherModel[false] = AssetManager::Instance().GetModelData(m_unLoadModelPath);
	}

	ImGui::InputText("BulletPath", &m_bulletPath);
	if (ImGui::SameLine(); ImGui::Button("Input"))
	{
		m_bullet = GameObjectManager::CreateObject(m_bulletPath, false);
	}
}

void Cp_Launcher::InitJson()
{
}

nlohmann::json Cp_Launcher::GetJson()
{
	m_jsonData["loadedModelPath"] = m_loadedModelPath;
	m_jsonData["unLoadModelPath"] = m_unLoadModelPath;

	m_jsonData["bulletPath"]	  = m_bulletPath;
	return m_jsonData;
}
