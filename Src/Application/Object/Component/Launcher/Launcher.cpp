#include "Launcher.h"
#include "../../Game/GameObject.h"
#include "../../Game/Manager/GameObjectManager.h"
#include "../../../SceneBase/SceneBase.h"
#include "../../../SceneBase/Manager/SceneManager.h"

void Cp_Launcher::Start()
{
	m_bullet = SceneManager::Instance().GetNowScene().lock()->GetGameObject()
		.CreateObject(m_bulletPath, false);
	m_bullet->SetHideFlg(true);
	m_bullet->SetActive(false);
}

void Cp_Launcher::PreUpdateContents()
{
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000 && m_cartridge.size())
	{
		m_cartridge.begin()->lock()->Destroy();
		m_cartridge.erase(m_cartridge.begin());
	}
	if (GetAsyncKeyState('R') & 0x8000 && m_cartridge.empty())
	{
		std::weak_ptr<GameObject> object = m_bullet->Initialize(m_owner);
		object.lock()->SetHideFlg(false);
		object.lock()->SetActive(true);
		m_cartridge.push_back(object);
		m_capacity++;
	}
}

void Cp_Launcher::ImGuiUpdate()
{
	ImGui::InputText("bulletPath", &m_bulletPath);
	if (ImGui::SameLine(); ImGui::Button("Input"))
	{
		m_bullet = SceneManager::Instance().GetNowScene().lock()
			->GetGameObject().CreateObject(m_bulletPath, false);
	}


	if (m_bullet)
	{
		ImGui::BeginChild("myBullet");
		{
			ImGui::Text(m_bullet->GetName().c_str());
			m_bullet->ImGuiComponents();
		}
		ImGui::EndChild();
	}
}

void Cp_Launcher::InitJson()
{
}

nlohmann::json Cp_Launcher::GetJson()
{
	return nlohmann::json();
}
