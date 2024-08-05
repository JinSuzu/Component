#include "MoveLocusComponent.h"
#include "../../../Game/GameObject.h"

#include "../../../../Utility/CreateObject/CreateObject.h"
#include "../../../../Utility/Timer.h"

void  MoveLocusComponent::Start()
{
	m_locus = std::make_shared<CreateObject>(m_owner);
	m_rigid = m_owner.lock()->GetComponent<RigidbodyComponent>();
}

void  MoveLocusComponent::PreUpdateContents()
{
	if (!Timer::Instance().GetAlarm(m_interval)) return;
	if(m_rigid.lock()->GetMove().LengthSquared())m_locus->Create();
}

void  MoveLocusComponent::ImGuiUpdate()
{
	ImGui::InputInt("Interval",&m_interval);
	m_locus->ImGuiUpdate();
}

void MoveLocusComponent::LoadJson(nlohmann::json _json)
{
	m_interval =  _json["Interval"];
	m_locus->SetJson(_json);
}

nlohmann::json  MoveLocusComponent::GetJson()
{
	nlohmann::json json;
	m_locus->OutJson(json);
	json["Interval"] = m_interval;
	return json;
}