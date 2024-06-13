#include "Player.h"
#include "../../Game/GameObject.h"
#include "../Transform/Transform.h"
#include "../Rigidbody/Rigidbody.h"

#include "../../../SceneBase/Manager/SceneManager.h"
#include "../../../SceneBase/SceneBase.h"
#include "../../Game/Manager/GameObjectManager.h"

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

	KdCollider::SphereInfo sphereInfo
	(
		KdCollider::Type::TypeDamage,
		m_trans.lock()->GetPosition(),
		m_rigid.lock()->GetHeight()
	);

	std::list<KdCollider::CollisionResult> results;
	SceneManager::Instance().GetNowScene().lock()->GetGameObject().RayHit(sphereInfo, &results);
	for (auto& result : results)
	{
		//Math::Vector3 overMove = (result.m_hitPos - result.m_objectPos);
		Math::Vector3 overMove = (m_trans.lock()->GetPosition() - result.m_objectPos);
		overMove.Normalize();

		Math::Vector3 move = m_rigid.lock()->GetMove();
		move.y = 0;
		m_rigid.lock()->SetMove(move);
		m_rigid.lock()->AddMove(overMove * m_reflectPow);
		m_rigid.lock()->SetGravity(0);
	}
}

void Cp_Player::PostUpdateContents()
{
	if (m_accelerationTimeCnt == m_accelerationTime) m_jumpFlg = !m_rigid.lock()->GetLanding();
}

void Cp_Player::ImGuiUpdate()
{
	ImGui::DragFloat("JumpPow", &m_jumpPow);
	ImGui::DragInt("AccelerationTime", &m_accelerationTime);
	ImGui::DragFloat("ReflectPow", &m_reflectPow);
}

void Cp_Player::InitJson()
{
	m_jumpPow = m_jsonData["jumpFlg"];
	m_accelerationTime = m_jsonData["AccelerationTime"];
	if(m_jsonData["ReflectPow"].is_number_float())m_reflectPow = m_jsonData["ReflectPow"];
}

nlohmann::json Cp_Player::GetJson()
{
	m_jsonData["jumpFlg"] = m_jumpPow;
	m_jsonData["AccelerationTime"] = m_accelerationTime;
	m_jsonData["ReflectPow"] = m_reflectPow;
	return m_jsonData;
}
