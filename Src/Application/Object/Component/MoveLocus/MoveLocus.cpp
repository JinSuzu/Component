#include "MoveLocus.h"
#include "../../Game/GameObject.h"

#include "../Rigidbody/Rigidbody.h"
#include "../Transform/Transform.h"

#include "../../../Utility/CreateObject/CreateObject.h"
#include "../../../Utility/Timer.h"

void  Cp_MoveLocus::Start()
{
	m_locus = std::make_shared<CreateObject>(m_owner);
	m_rigid = m_owner.lock()->GetComponent<Cp_Rigidbody>();
}

void  Cp_MoveLocus::PreUpdateContents()
{
	if (!Timer::Instance().GetAlarm(m_interval)) return;
	if(m_rigid.lock()->GetMove().LengthSquared())m_locus->Create();
}

void  Cp_MoveLocus::ImGuiUpdate()
{
	ImGui::InputInt("Interval",&m_interval);
	m_locus->ImGuiUpdate();
}

void Cp_MoveLocus::LoadJson(nlohmann::json _json)
{
	m_interval =  _json["Interval"];
	m_locus->SetJson(_json);
}

nlohmann::json  Cp_MoveLocus::GetJson()
{
	nlohmann::json json;
	m_locus->OutJson(json);
	json["Interval"] = m_interval;
	return json;
}