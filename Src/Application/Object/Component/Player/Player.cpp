#include "Player.h"
#include "../../Game/GameObject.h"
#include "../Transform/Transform.h"
#include "../Rigidbody/Rigidbody.h"

#include "../../../SceneBase/Manager/SceneManager.h"
#include "../../../SceneBase/SceneBase.h"
#include "../../Game/Manager/GameObjectManager.h"

#include "../../../main.h"

void Cp_Player::Start()
{
	m_trans = m_owner.lock()->GetTransform();
	m_rigid = m_owner.lock()->GetComponent<Cp_Rigidbody>();
	assert(m_rigid.lock() && "リジットないよbyPlayer");
}

void Cp_Player::PreUpdateContents()
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

void Cp_Player::UpdateContents()
{
}

void Cp_Player::PostUpdateContents()
{
	if (m_accelerationTimeCnt == m_accelerationTime) m_jumpFlg = !m_rigid.lock()->GetLanding();
	for (auto& result : m_rigid.lock()->GetHitResult())
	{
		if (!(result.m_type & (UINT)KdCollider::Type::TypeBlock)) continue;
		m_trans.lock()->SetPosition(result.m_hitPos);
	}
}

void Cp_Player::ImGuiUpdate()
{
	ImGui::DragFloat("JumpPow", &m_jumpPow);
	ImGui::DragInt("AccelerationTime", &m_accelerationTime);
}

void Cp_Player::LoadJson(nlohmann::json _json)
{
	m_jumpPow = _json["jumpFlg"];
	m_accelerationTime = _json["AccelerationTime"];
}

nlohmann::json Cp_Player::GetJson()
{
	nlohmann::json json;
	json["jumpFlg"] = m_jumpPow;
	json["AccelerationTime"] = m_accelerationTime;
	return json;
}
