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

void  Cp_MoveLocus::InitJson()
{
	m_interval =  m_jsonData["Interval"];
	m_locus->SetJson(m_jsonData);
}

nlohmann::json  Cp_MoveLocus::GetJson()
{
	m_locus->OutJson(m_jsonData);
	 m_jsonData["Interval"] = m_interval;
	return m_jsonData;
}