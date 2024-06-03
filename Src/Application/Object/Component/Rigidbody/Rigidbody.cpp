#include "Rigidbody.h"
#include "../../Game/GameObject.h"
#include "../../../SceneBase/Manager/SceneManager.h"
#include "../../../SceneBase/SceneBase.h"
#include "../Transform/Transform.h"

void Cp_Rigidbody::Start()
{

}

void Cp_Rigidbody::InitJson()
{
	if (!m_jsonData.is_null())
	{
		m_move = JsonToVec3(m_jsonData["move"]);
		m_gravity = JsonToVec3(m_jsonData["gravity"]);
	}
}

void Cp_Rigidbody::PreUpdateContents()
{
}

void Cp_Rigidbody::UpdateContents()
{
	m_move -= m_gravity;
	std::shared_ptr<Cp_Transform>tans =  m_owner.lock()->GetTransform().lock();
	tans->SetPosition(tans->GetPosition() + m_move);

	KdCollider::RayInfo rayInfo(
		KdCollider::Type::TypeGround,
		m_owner.lock()->GetTransform().lock()->GetMatrix().Translation(),
		m_move,
		m_move.Length()
	);

	std::list<KdCollider::CollisionResult>results;
	SceneManager::Instance().GetNowScene().lock()->GetGameObject().RayHit(rayInfo, &results);
	for (auto& result : results) 
	{
		tans->SetPosition(result.m_hitPos);
		if (result.m_hitDir.x)m_move.x = 0;
		if (result.m_hitDir.y)m_move.y = 0;
		if (result.m_hitDir.z)m_move.z = 0;
	}
}

void Cp_Rigidbody::PostUpdateContents()
{
	m_move *= 0.85;
}

void Cp_Rigidbody::ImGuiUpdate()
{
	ImGui::DragFloat3("move",&m_move.x);
	ImGui::DragFloat3("gravity",&m_gravity.x);
}

nlohmann::json Cp_Rigidbody::GetJson()
{
	m_jsonData["move"]	= Vec3ToJson(m_move);
	m_jsonData["gravity"] = Vec3ToJson(m_gravity);
	return m_jsonData;
}
