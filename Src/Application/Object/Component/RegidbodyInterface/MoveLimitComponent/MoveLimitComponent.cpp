#include "MoveLimitComponent.h"
#include "../../../Game/GameObject.h"

void MoveLimitComponent::Start()
{
	m_trans = m_owner.lock()->GetTransform();
}

void MoveLimitComponent::PreUpdateContents()
{
	if (m_startPoint == Math::Vector3::Zero)m_startPoint = m_trans.lock()->GetMatrix().Translation();
	float movingDistance = (m_trans.lock()->GetMatrix().Translation() - m_startPoint).Length();
	if (movingDistance > m_shotRange) 
	{
		m_owner.lock()->Destroy();
	}
}

void MoveLimitComponent::ImGuiUpdate()
{
	ImGui::DragFloat("ShotRange", &m_shotRange);
	float movingDistance = (m_trans.lock()->GetLocalPosition(), m_startPoint).Length();
	ImGui::DragFloat("Range", &movingDistance);
}

void MoveLimitComponent::LoadJson(nlohmann::json _json)
{
	if(_json["ShotRange"].is_number())m_shotRange = _json["ShotRange"];
}

nlohmann::json MoveLimitComponent::Serialize()
{
	nlohmann::json json;
	json["ShotRange"] = m_shotRange;
	return json;
}
