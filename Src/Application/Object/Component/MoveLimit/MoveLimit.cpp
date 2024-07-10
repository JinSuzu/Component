#include "MoveLimit.h"
#include "../../Game/GameObject.h"
#include "../Transform/Transform.h"

void Cp_MoveLimit::Start()
{
	m_trans = m_owner.lock()->GetTransform();
}

void Cp_MoveLimit::PreUpdateContents()
{
	if (m_startPoint == Math::Vector3::Zero)m_startPoint = m_trans.lock()->GetMatrix().Translation();
	float movingDistance = (m_trans.lock()->GetMatrix().Translation() - m_startPoint).Length();
	if (movingDistance > m_shotRange) 
	{
		m_owner.lock()->Destroy();
	}
}

void Cp_MoveLimit::ImGuiUpdate()
{
	ImGui::DragFloat("ShotRange", &m_shotRange);
	float movingDistance = (m_trans.lock()->GetPosition(), m_startPoint).Length();
	ImGui::DragFloat("Range", &movingDistance);
}

void Cp_MoveLimit::LoadJson(nlohmann::json _json)
{
	if(_json["ShotRange"].is_number())m_shotRange = _json["ShotRange"];
}

nlohmann::json Cp_MoveLimit::GetJson()
{
	nlohmann::json json;
	json["ShotRange"] = m_shotRange;
	return json;
}
