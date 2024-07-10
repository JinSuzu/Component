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

void Cp_HitResultDestroy::LoadJson(nlohmann::json _json)
{
	Cp_HitResult::LoadJson(_json);
	m_sePath = _json["SEPath"];
}

nlohmann::json Cp_HitResultDestroy::GetJson()
{
	nlohmann::json json = Cp_HitResult::GetJson();
	json["SEPath"]		= m_sePath;
	return json;
}

void Cp_HitResultDestroy::Destroy()
{
	KdAudioManager::Instance().Play("Asset/Sounds/" + m_sePath + ".wav")
		->SetVolume(2.0f);
}

