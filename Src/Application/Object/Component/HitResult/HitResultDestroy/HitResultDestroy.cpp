#include "../../../Game/GameObject.h"
#include "HitResultDestroy.h"

#include "../../Rigidbody/Rigidbody.h"
#include "../../Transform/Transform.h"

void Cp_HitResultDestroy::UpdateContents()
{
	if (!m_rigid.lock()->GetHitResult().empty())
	{
		m_owner.lock()->Destroy();
	}
}

void Cp_HitResultDestroy::ImGuiUpdate()
{
	Cp_HitResult::ImGuiUpdate();
	ImGui::InputText("SEPath", &m_sePath);
}

void Cp_HitResultDestroy::InitJson()
{
	Cp_HitResult::InitJson();
	m_sePath = m_jsonData["SEPath"];
}

nlohmann::json Cp_HitResultDestroy::GetJson()
{
	Cp_HitResult::GetJson();
	m_jsonData["SEPath"] = m_sePath;
	return m_jsonData;
}

void Cp_HitResultDestroy::Destroy()
{
	KdAudioManager::Instance().Play("Asset/Sounds/" + m_sePath + ".wav")
		->SetVolume(2.0f);
}

