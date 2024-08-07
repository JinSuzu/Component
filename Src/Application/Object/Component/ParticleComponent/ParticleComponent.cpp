#include "ParticleComponent.h"
#include "../../Game/GameObject.h"

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

void Cp_Particle::LoadJson(nlohmann::json _json)
{
	if (_json["DeleteTime"].is_number())m_deleteTime = _json["DeleteTime"];
}

nlohmann::json Cp_Particle::Serialize()
{
	nlohmann::json json;
	json["DeleteTime"] = m_deleteTime;
	return json;
}
