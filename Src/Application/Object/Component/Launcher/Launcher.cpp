#include "Launcher.h"
#include "../../Game/GameObject.h"
#include "../../Game/Manager/GameObjectManager.h"
#include "../../../SceneBase/SceneBase.h"
#include "../../../SceneBase/Manager/SceneManager.h"

void Cp_Launcher::Start()
{
}

void Cp_Launcher::PreUpdateContents()
{
	if (GetAsyncKeyState('R') & 0x8000 && m_capacity == 0) 
	{
		//m_owner.lock()->WorkFamily()->
	}
}

void Cp_Launcher::ImGuiUpdate()
{
	ImGui::InputText("bulletPath", &m_bulletPath);
	if (ImGui::SameLine(); ImGui::Button("Input"))
	{
		/*m_bullet = SceneManager::Instance().GetNowScene().lock()->GetGameObject()
				.CreateObject(m_bulletPath, nullptr, false);*/
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
