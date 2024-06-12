#include "Rigidbody.h"
#include "../../Game/GameObject.h"
#include "../../../SceneBase/Manager/SceneManager.h"
#include "../../../SceneBase/SceneBase.h"
#include "../Transform/Transform.h"


void Cp_Rigidbody::Start()
{
	m_trans = m_owner.lock()->GetTransform();
}

void Cp_Rigidbody::PreUpdateContents()
{
	if(m_bActiveGravity && m_gravity < 2.5)m_gravity -= m_gravityPow;
}

void Cp_Rigidbody::UpdateContents()
{
	if(m_bActiveGravity)m_move.y = Gravity();
	m_trans.lock()->SetPosition(m_trans.lock()->GetPosition() + m_move);

	m_move *= m_deceleration;
}


void Cp_Rigidbody::ImGuiUpdate()
{
	ImGui::DragFloat3("Move", &m_move.x);

	if (ImGui::Checkbox("Gravity", &m_bActiveGravity); m_bActiveGravity)
	{
		ImGui::DragFloat("GravityPow", &m_gravityPow);
		ImGui::DragFloat("Height", &m_height);
	}

	ImGui::DragFloat("Deceleration", &m_deceleration);
}

void Cp_Rigidbody::InitJson()
{
	m_move = JsonToVec3(m_jsonData["move"]);

	m_bActiveGravity = m_jsonData["activeGravityFlag"];
	m_gravityPow = m_jsonData["gravity"];
	m_height = m_jsonData["height"];

	m_deceleration = m_jsonData["deceleration"];
}

nlohmann::json Cp_Rigidbody::GetJson()
{
	m_jsonData["move"] = Vec3ToJson(m_move);

	m_jsonData["activeGravityFlag"] = m_bActiveGravity;
	m_jsonData["gravity"] = m_gravityPow;
	m_jsonData["height"] = m_height;

	m_jsonData["deceleration"] = m_deceleration;
	return m_jsonData;
}

float Cp_Rigidbody::Gravity()
{
	Math::Vector3 pos = m_owner.lock()->GetTransform().lock()->GetPosition();
	pos.y -= m_height;
	KdCollider::RayInfo rayInfo(
		KdCollider::Type::TypeGround,
		pos,
		{ 0,-1,0 },
		abs(m_gravity + m_move.y)
	);

	//重力と床判定
	m_bLanding = false;
	std::list<KdCollider::CollisionResult>	results;
	KdCollider::CollisionResult				effectResult;
	float									hitRange = 0.0f;

	SceneManager::Instance().GetNowScene().lock()->GetGameObject().RayHit(rayInfo, &results);
	for (auto& result : results)
	{
		int targetRange = Math::Vector3::Distance(result.m_hitPos, m_trans.lock()->GetPosition());
		if (hitRange > targetRange|| !m_bLanding)
		{
			hitRange = targetRange;
			effectResult = result;
			m_bLanding = true;
		}
	}

	float futureGravityPow = m_gravity + m_move.y;
	if (m_bLanding)
	{
		Math::Vector3 overMove = effectResult.m_overlapDistance * effectResult.m_hitDir;
		futureGravityPow += overMove.y;
		m_gravity = 0;
	}

	return futureGravityPow;
}
