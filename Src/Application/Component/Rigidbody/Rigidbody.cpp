#include "Rigidbody.h"
#include "../../Object/Object.h"
#include "../Transform/Transform.h"

void Cp_Rigidbody::Start(std::weak_ptr<Object> _owner)
{
	Component::Start(_owner);
	if (Json.is_object())
	{
		m_move = JsonToVec3(Json["move"]);
		m_gravity = JsonToVec3(Json["gravity"]);
	}
}

void Cp_Rigidbody::PreUpdate()
{
	m_move -= m_gravity;
}

void Cp_Rigidbody::Update()
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
	Json["move"]	= Vec3ToJson(m_move);
	Json["gravity"] = Vec3ToJson(m_gravity);
	return Json;
}
