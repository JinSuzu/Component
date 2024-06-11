#include "Rigidbody.h"
#include "../../Game/GameObject.h"
#include "../../../SceneBase/Manager/SceneManager.h"
#include "../../../SceneBase/SceneBase.h"
#include "../Transform/Transform.h"


void Cp_Rigidbody::Start()
{
	m_trans = m_owner.lock()->GetTransform();
}

void Cp_Rigidbody::UpdateContents()
{
	Math::Vector3 postPos = m_move;
	if (m_bActiveGravity)
	{
		postPos.y += Gravity();
	}

	m_trans.lock()->SetPosition(m_trans.lock()->GetPosition() + postPos);
	m_move *= m_deceleration;
}


void Cp_Rigidbody::ImGuiUpdate()
{
	ImGui::DragFloat3("move", &m_move.x);

	if (ImGui::Checkbox("gravity", &m_bActiveGravity); m_bActiveGravity)
	{
		ImGui::DragFloat("gravity", &m_gravityPow);
		ImGui::DragFloat("height", &m_height);
	}


	ImGui::DragFloat("deceleration", &m_deceleration);
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
	m_gravity -= m_gravityPow;

	KdCollider::RayInfo rayInfo(
		KdCollider::Type::TypeGround,
		m_owner.lock()->GetTransform().lock()->GetPosition(),
		{ 0,-1,0 },
		abs(m_gravity) + m_height
	);

	//重力と床判定
	bool standingGroundFlg = false;
	std::list<KdCollider::CollisionResult>	results;
	KdCollider::CollisionResult				effectResult;
	float									hitRange = 0.0f;

	SceneManager::Instance().GetNowScene().lock()->GetGameObject().RayHit(rayInfo, &results);
	for (auto& result : results)
	{
		if (hitRange > Math::Vector3::Distance(result.m_hitPos, m_trans.lock()->GetPosition()) || !standingGroundFlg)
		{
			standingGroundFlg = true;
			effectResult = result;
		}
	}

	float gravityPow = m_gravity;
	if (standingGroundFlg)
	{
		Math::Vector3 overMove = effectResult.m_overlapDistance * effectResult.m_hitDir;
		gravityPow += overMove.y;
		m_gravity = 0;
	}
	
	return gravityPow;
}
