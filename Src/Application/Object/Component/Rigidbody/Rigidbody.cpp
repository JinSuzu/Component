#include "Rigidbody.h"
#include "../../Game/GameObject.h"
#include "../../../SceneBase/Manager/SceneManager.h"
#include "../../../SceneBase/SceneBase.h"
#include "../Transform/Transform.h"

#include "../../../RenderManger/RenderManger.h"

void Cp_Rigidbody::Start()
{
	m_trans = m_owner.lock()->GetTransform();
}

void Cp_Rigidbody::PreUpdateContents()
{
	if (m_bActiveGravity && m_gravity < 2.5)m_gravity -= m_gravityPow;
}

void Cp_Rigidbody::UpdateContents()
{
	if (m_bActiveGravity)m_move.y = Gravity();
	m_trans.lock()->SetPosition(m_trans.lock()->GetPosition() + m_move);
	MakeResults();

	m_move *= m_deceleration;
}


void Cp_Rigidbody::ImGuiUpdate()
{
	ImGui::DragFloat3("Move", &m_move.x);

	if (ImGui::Checkbox("Gravity", &m_bActiveGravity); m_bActiveGravity)
	{
		ImGui::DragFloat("GravityPow", &m_gravityPow);
	}

	if (ImGui::Button("ShapeBody"))ImGui::OpenPopup(("ShapeBody##" + std::to_string(m_instanceID)).c_str());
	if (ImGui::BeginPopup(("ShapeBody##" + std::to_string(m_instanceID)).c_str()))
	{
		for (int i = 0; i < Shape::Max; i++)
		{
			Shape shape = static_cast<Shape>(i);
			ImGui::RadioButton(magic_enum::enum_name(shape).data(), (int*)&m_shape, shape);
		}
		ImGui::EndPopup();
	}
	switch (m_shape)
	{
	case Cp_Rigidbody::Sphere:
		ImGui::DragFloat("Radius", &m_shapeDate.radius.y);
		break;
	case Cp_Rigidbody::Box:
		ImGui::DragFloat3("Radius", &m_shapeDate.radius.x);
		break;
	case Cp_Rigidbody::Ray:
		ImGui::DragFloat("Radius", &m_shapeDate.radius.y);
		break;
	default:
		break;
	}
	ImGui::DragFloat3("OffsetPos", &m_shapeDate.offestPos.x);
	if (ImGui::Button("ColliderType"))ImGui::OpenPopup(("ColliderType##" + std::to_string(m_instanceID)).c_str());
	if (ImGui::BeginPopup(("ColliderType##" + std::to_string(m_instanceID)).c_str()))
	{
		for (int i = 0; i < std::log2((double)KdCollider::Type::TypeMax); i++)
		{
			KdCollider::Type type = static_cast<KdCollider::Type>(1 << i);
			ImGuiCheckBoxBit(magic_enum::enum_name(type).data(), m_shapeDate.tag, type);
		}
		ImGui::EndPopup();
	}

	ImGui::DragFloat("Deceleration", &m_deceleration);
}

void Cp_Rigidbody::InitJson()
{
	m_move = JsonToVec3(m_jsonData["move"]);

	m_bActiveGravity = m_jsonData["activeGravityFlag"];
	m_gravityPow = m_jsonData["gravity"];

	m_deceleration = m_jsonData["deceleration"];

	if (m_jsonData["Shape"].is_number()) m_shape = m_jsonData["Shape"];
	m_shapeDate.radius = JsonToVec3(m_jsonData["Radius"]);
	m_shapeDate.offestPos = JsonToVec3(m_jsonData["OffsetPos"]);
	m_shapeDate.tag = m_jsonData["Tag"];
}

nlohmann::json Cp_Rigidbody::GetJson()
{
	m_jsonData["move"] = Vec3ToJson(m_move);

	m_jsonData["activeGravityFlag"] = m_bActiveGravity;
	m_jsonData["gravity"] = m_gravityPow;

	m_jsonData["deceleration"] = m_deceleration;

	m_jsonData["Shape"] = m_shape;
	m_jsonData["Radius"] = Vec3ToJson(m_shapeDate.radius);
	m_jsonData["OffsetPos"] = Vec3ToJson(m_shapeDate.offestPos);
	m_jsonData["Tag"] = m_shapeDate.tag;
	return m_jsonData;
}

float Cp_Rigidbody::Gravity()
{
	Math::Vector3 pos = m_owner.lock()->GetTransform().lock()->GetPosition();
	pos.y -= m_shapeDate.radius.y;
	pos.y += m_shapeDate.offestPos.y;
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
		if (hitRange > targetRange || !m_bLanding)
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

void Cp_Rigidbody::MakeResults()
{
	m_shapeDate.pResults.clear();

	if (m_shape == Cp_Rigidbody::Sphere)
	{
		KdCollider::SphereInfo SphereInfo
		(
			m_shapeDate.tag,
			m_trans.lock()->GetPosition() + m_shapeDate.offestPos,
			m_shapeDate.radius.y
		);
		SceneManager::Instance().GetNowScene().lock()->GetGameObject().RayHit(SphereInfo, &m_shapeDate.pResults);
		RenderManager::Instance().m_m_debugWireFrame.AddDebugSphere(m_trans.lock()->GetPosition() + m_shapeDate.offestPos, m_shapeDate.radius.y);
	}

	else if (m_shape == Cp_Rigidbody::Box)
	{
		KdCollider::BoxInfo BoxInfo
		(
			m_shapeDate.tag,
			m_trans.lock()->GetMatrix(),
			m_shapeDate.offestPos,
			m_shapeDate.radius,
			false
		);
		SceneManager::Instance().GetNowScene().lock()->GetGameObject().RayHit(BoxInfo, &m_shapeDate.pResults);
		RenderManager::Instance().m_m_debugWireFrame.AddDebugBox(m_trans.lock()->GetMatrix(), m_shapeDate.radius, m_shapeDate.offestPos);
	}

	else if (m_shape == Cp_Rigidbody::Ray)
	{
		KdCollider::RayInfo rayInfo
		(
			m_shapeDate.tag,
			m_trans.lock()->GetPosition() + m_shapeDate.offestPos,
			m_trans.lock()->GetPosition() + m_shapeDate.offestPos + m_move,
			m_move.Length()
		);
		SceneManager::Instance().GetNowScene().lock()->GetGameObject().RayHit(rayInfo, &m_shapeDate.pResults);
		RenderManager::Instance().m_m_debugWireFrame.AddDebugLine(rayInfo.m_pos, rayInfo.m_dir, rayInfo.m_range);
	}

}
