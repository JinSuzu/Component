#include "../../../Game/GameObject.h"
#include "HitResultReflectComponent.h"

void HitResultReflectComponent::UpdateContents()
{
	for (auto& result : m_rigid.lock()->GetHitResult())
	{
		if (!(m_colType & result.m_type))continue;
		Math::Vector3 overMove = (m_trans.lock()->GetMatrix().Translation() + m_rigid.lock()->GetOffsetPos() - result.m_objectPos);
		overMove.Normalize();

		Math::Vector3 move = overMove * m_reflectPow;
		move.y = abs(move.y);
		m_rigid.lock()->AddMove(move);
		m_rigid.lock()->SetGravity(0);
		m_bFlying = true;
	}

	if (m_bFlying) 
	{
		if (!m_bPlay) 
		{
			KdAudioManager::Instance().Play("Asset/Sounds/Flging.wav");
			m_bPlay = true;
		}
	}
}

void HitResultReflectComponent::ImGuiUpdate()
{
	ImGui::DragFloat("ReflectPow", &m_reflectPow);
	Cp_HitResult::ImGuiUpdate();
}

void HitResultReflectComponent::LoadJson(nlohmann::json _json)
{
	Cp_HitResult::LoadJson(_json);
	if (_json["ReflectPow"].is_number_float())m_reflectPow = _json["ReflectPow"];
}

nlohmann::json HitResultReflectComponent::GetJson()
{
	nlohmann::json json = Cp_HitResult::GetJson();
	json["ReflectPow"] = m_reflectPow;
	return json;
}
