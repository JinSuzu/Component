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
		std::weak_ptr<GameObject> object = GameObjectManager::CreateObject(m_bulletPath);
		m_owner.lock()->AddChilds(object);
		m_capacity++;
	}

	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) 
	{
		std::list<std::weak_ptr<GameObject>>& list = m_owner.lock()->GetChilds();
		if(list.empty())list.erase(list.begin());
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
