#include "AddRotaion.h"
#include "../../Object/Object.h"
#include "../Transform/Transform.h"

void Cp_AddRotation::Start(std::weak_ptr<Object> _owner)
{
	Component::Start(_owner);
	m_wpTans = _owner.lock()->GetspTransform();

	if (Json.is_object())
	{
		m_addPow = JsonToVec3(Json["addPow"]);
	}
}

void Cp_AddRotation::Update()
{
	Math::Vector3  rota = m_wpTans.lock()->GetRotation() + m_addPow;
	
	rota.x = (int)rota.x % 360;
	rota.y = (int)rota.y % 360;
	rota.z = (int)rota.z % 360;

	m_wpTans.lock()->SetRotation(rota);
}

void Cp_AddRotation::ImGuiUpdate()
{
	float value[3]TOARRAY3(m_addPow);
	ImGui::DragFloat3("addPow",value);
	m_addPow = Math::Vector3(value);
	
}

nlohmann::json Cp_AddRotation::GetJson()
{
	nlohmann::json json;
	json["addPow"] = Vec3ToJson(m_addPow);
	return json;
}
