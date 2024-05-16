#include "AddRotaion.h"
#include "../../Game/GameObject.h"
#include "../Transform/Transform.h"

void Cp_AddRotation::Start()
{
	m_wpTans = m_owner.lock()->GetTransform();
}

void Cp_AddRotation::InitJson()
{
	Component::InitJson();
	if (m_jsonData.is_object())
	{
		m_addPow = JsonToVec3(m_jsonData["addPow"]);
	}
}

void Cp_AddRotation::UpdateContents()
{
	Math::Vector3  rota = m_wpTans.lock()->GetRotation() + m_addPow;
	
	rota.x = (int)rota.x % 360;
	rota.y = (int)rota.y % 360;
	rota.z = (int)rota.z % 360;

	m_wpTans.lock()->SetRotation(rota);
}

void Cp_AddRotation::ImGuiUpdate()
{
	ImGui::DragFloat3("addPow",&m_addPow.x);
}

nlohmann::json Cp_AddRotation::GetJson()
{
	m_jsonData["addPow"] = Vec3ToJson(m_addPow);
	return m_jsonData;
}
