﻿#include "Rigidbody.h"
#include "../../Game/GameObject.h"
#include "../Transform/Transform.h"

void Cp_Rigidbody::Start()
{

}

void Cp_Rigidbody::InitJson()
{
	Component::InitJson();
	if (!m_jsonData.is_null())
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
	std::weak_ptr<Cp_Transform>tans =  m_owner.lock()->GetTransform();
	tans.lock()->SetPosition(tans.lock()->GetPosition() + m_move);
}

void Cp_Rigidbody::PostUpdateContents()
{
}

void Cp_Rigidbody::ImGuiUpdate()
{
	ImGui::DragFloat3("move",&m_move.x);
	ImGui::DragFloat3("gravity",&m_gravity.x);
}

nlohmann::json Cp_Rigidbody::GetJson()
{
	m_jsonData["move"]	= Vec3ToJson(m_move);
	m_jsonData["gravity"] = Vec3ToJson(m_gravity);
	return m_jsonData;
}
