#include "PlayerComponent.h"
#include "../../Game/GameObject.h"

#include "../../Game/Manager/GameObjectManager.h"

#include "../../../main.h"

void PlayerComponent::Start()
{
	m_trans = m_owner.lock()->GetTransform();
	m_rigid = m_owner.lock()->GetComponent<RigidbodyComponent>();
	assert(m_rigid.lock() && "リジットないよbyPlayer");
}

void PlayerComponent::PreUpdateContents()
{
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		if (!m_jumpFlg)
		{
			m_jumpFlg = true;
			m_accelerationTimeCnt = 0;
			m_rigid.lock()->SetGravity(0);
		}
	}

	if (m_jumpFlg)
	{
		if (m_accelerationTimeCnt != m_accelerationTime)
		{
			m_rigid.lock()->AddMove(Math::Vector3(0, m_jumpPow * (m_accelerationTime / (float)(m_accelerationTimeCnt + 1)), 0));
			m_accelerationTimeCnt++;
		}
	}
}

void PlayerComponent::UpdateContents()
{
}

void PlayerComponent::PostUpdateContents()
{
	if (m_accelerationTimeCnt == m_accelerationTime) m_jumpFlg = !m_rigid.lock()->GetLanding();
	for (auto& result : m_rigid.lock()->GetHitResult())
	{
		if (!(result.m_type & (UINT)KdCollider::Type::TypeBlock)) continue;
		m_trans.lock()->SetLoaclPosition(result.m_hitPos);
	}
}

void PlayerComponent::ImGuiUpdate()
{
	ImGui::DragFloat("JumpPow", &m_jumpPow);
	ImGui::DragInt("AccelerationTime", &m_accelerationTime);
}

void PlayerComponent::LoadJson(nlohmann::json _json)
{
	m_jumpPow = _json["jumpFlg"];
	m_accelerationTime = _json["AccelerationTime"];
}

nlohmann::json PlayerComponent::Serialize()
{
	nlohmann::json json;
	json["jumpFlg"] = m_jumpPow;
	json["AccelerationTime"] = m_accelerationTime;
	return json;
}
