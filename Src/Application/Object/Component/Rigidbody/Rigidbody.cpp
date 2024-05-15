#include "Rigidbody.h"
#include "../../Game/GameObject.h"
#include "../Transform/Transform.h"

void Cp_Rigidbody::Start()
{

}

void Cp_Rigidbody::InitJson()
{
	Component::InitJson();
	if (m_jsonData.is_object())
	{
		m_move = JsonToVec3(m_jsonData["move"]);
		m_gravity = JsonToVec3(m_jsonData["gravity"]);
	}
}

void Cp_Rigidbody::PreUpdateContents()
{
	m_move -= m_gravity;
}

void Cp_Rigidbody::UpdateContents()
{
	std::weak_ptr<Cp_Transform>tans =  m_owner.lock()->GetspTransform();
	tans.lock()->SetPosition(tans.lock()->GetPosition() + m_move);
}

void Cp_Rigidbody::ImGuiUpdate()
{
	float vol1[3]TOARRAY3(m_move);
	ImGui::DragFloat3("move",vol1);
	m_move = Math::Vector3(vol1);

	float vol2[3]TOARRAY3(m_gravity);
	ImGui::DragFloat3("gravity",vol2);
	m_gravity = Math::Vector3(vol2);
}

nlohmann::json Cp_Rigidbody::GetJson()
{
	m_jsonData["move"]	= Vec3ToJson(m_move);
	m_jsonData["gravity"] = Vec3ToJson(m_gravity);
	return m_jsonData;
}
