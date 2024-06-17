#include "Particle.h"
#include "../../Game/GameObject.h"
#include "../Transform/Transform.h"

void Cp_Particle::Start()
{
	std::weak_ptr<GameObject> parent = m_owner.lock()->GetParent();
	parent.lock();
}

void Cp_Particle::PreUpdateContents()
{
	if (m_deleteTime <= m_deleteTimeCnt)
	{
		m_owner.lock()->Destroy();
		return;
	}
	m_deleteTimeCnt++;
}

void Cp_Particle::ImGuiUpdate()
{
	ImGui::DragInt("DeleteTimeCnt", &m_deleteTime);
}

void Cp_Particle::InitJson()
{
	if (m_jsonData["DeleteTime"].is_number())m_deleteTime = m_jsonData["DeleteTime"];
}

nlohmann::json Cp_Particle::GetJson()
{
	m_jsonData["DeleteTime"] = m_deleteTime;
	return m_jsonData;
}
