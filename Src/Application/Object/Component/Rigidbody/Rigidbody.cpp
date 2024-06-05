#include "Rigidbody.h"
#include "../../Game/GameObject.h"
#include "../../../SceneBase/Manager/SceneManager.h"
#include "../../../SceneBase/SceneBase.h"
#include "../Transform/Transform.h"


void Cp_Rigidbody::UpdateContents()
{
	m_move.y -= m_gravity;
}

void Cp_Rigidbody::PostUpdateContents()
{
	KdCollider::RayInfo rayInfo(
		KdCollider::Type::TypeGround,
		m_owner.lock()->GetTransform().lock()->GetPosition(),
		{ 0,-1,0 },
		5
	);

	std::list<KdCollider::CollisionResult>results;
	std::shared_ptr<Cp_Transform>tans = m_owner.lock()->GetTransform().lock();
	SceneManager::Instance().GetNowScene().lock()->GetGameObject().RayHit(rayInfo, &results);
	for (auto& result : results)
	{
		Math::Vector3 overMove = result.m_overlapDistance* result.m_hitDir;
		m_move += overMove;
	}
	tans->SetPosition(tans->GetPosition() + m_move);
	m_move *= m_deceleration;
}

void Cp_Rigidbody::ImGuiUpdate()
{
	ImGui::DragFloat3("move", &m_move.x);
	ImGui::DragFloat("gravity", &m_gravity);
	ImGui::DragFloat("deceleration", &m_deceleration);
}

void Cp_Rigidbody::InitJson()
{
	m_move = JsonToVec3(m_jsonData["move"]);
	m_gravity = m_jsonData["gravity"];
	m_deceleration = m_jsonData["deceleration"];
}

nlohmann::json Cp_Rigidbody::GetJson()
{
	m_jsonData["move"] = Vec3ToJson(m_move);
	m_jsonData["gravity"] = m_gravity;
	m_jsonData["deceleration"] = m_deceleration;
	return m_jsonData;
}