#include "../../../Game/GameObject.h"
#include "HitResultReflect.h"

#include "../../Rigidbody/Rigidbody.h"
#include "../../Transform/Transform.h"

void Cp_HitResultReflect::UpdateContents()
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

void Cp_HitResultReflect::ImGuiUpdate()
{
	ImGui::DragFloat("ReflectPow", &m_reflectPow);
	Cp_HitResult::ImGuiUpdate();
}

void Cp_HitResultReflect::InitJson()
{
	Cp_HitResult::InitJson();
	if (m_jsonData["ReflectPow"].is_number_float())m_reflectPow = m_jsonData["ReflectPow"];
}

nlohmann::json Cp_HitResultReflect::GetJson()
{
	Cp_HitResult::GetJson();
	m_jsonData["ReflectPow"] = m_reflectPow;
	return m_jsonData;
}
