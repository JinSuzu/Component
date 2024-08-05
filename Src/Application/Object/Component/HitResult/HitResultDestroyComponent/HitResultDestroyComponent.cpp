#include "../../../Game/GameObject.h"
#include "HitResultDestroyComponent.h"

void HitResultDestroyComponent::UpdateContents()
{
	if (!m_rigid.lock()->GetHitResult().empty())
	{
		m_owner.lock()->Destroy();
	}
}

void HitResultDestroyComponent::ImGuiUpdate()
{
	Cp_HitResult::ImGuiUpdate();
	ImGui::InputText("SEPath", &m_sePath);
}

void HitResultDestroyComponent::LoadJson(nlohmann::json _json)
{
	Cp_HitResult::LoadJson(_json);
	m_sePath = _json["SEPath"];
}

nlohmann::json HitResultDestroyComponent::GetJson()
{
	nlohmann::json json = Cp_HitResult::GetJson();
	json["SEPath"]		= m_sePath;
	return json;
}

void HitResultDestroyComponent::Destroy()
{
	KdAudioManager::Instance().Play("Asset/Sounds/" + m_sePath + ".wav")
		->SetVolume(2.0f);
}

