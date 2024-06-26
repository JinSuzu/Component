﻿#include "Rigidbody.h"
#include "../../Game/GameObject.h"
#include "../../../SceneBase/Manager/SceneManager.h"
#include "../../../SceneBase/SceneBase.h"
#include "../Transform/Transform.h"

#include "../../../RenderManger/RenderManger.h"
#include "../../../ImGuiHelper/ImGuiHelper.h"

void Cp_Rigidbody::Start()
{
	m_trans = m_owner.lock()->GetTransform();
	m_debugDraw = std::make_shared<std::function<void()>>([this]() {DrawDebug(); });
	RenderManager::Instance().AddDrawDebug(m_debugDraw);
}

void Cp_Rigidbody::PreUpdateContents()
{
	if (m_bActiveGravity && m_gravity < 2.5)m_gravity -= m_gravityPow;
}

void Cp_Rigidbody::UpdateContents()
{
	if (m_bActiveGravity)m_move.y = Gravity();
	if (m_collisionBody)MakeResults();
}

void Cp_Rigidbody::PostUpdateContents()
{
	m_trans.lock()->SetPosition(m_trans.lock()->GetPosition() + m_move);
	m_move *= m_deceleration;
}


void Cp_Rigidbody::ImGuiUpdate()
{
	ImGui::DragFloat3("Move", &m_move.x);
	ImGui::DragFloat("Deceleration", &m_deceleration);

	if (ImGui::Checkbox("Gravity", &m_bActiveGravity); m_bActiveGravity)
	{
		ImGui::DragFloat("GravityPow", &m_gravityPow);
		ImGui::DragFloat("Height", &m_height);
		ImGui::DragFloat("OffsetHeight", &m_offsetHeight);
	}

	if (ImGui::Checkbox("CollisionBody", &m_collisionBody); !m_collisionBody)return;
	if (ImGui::Button("ShapeBody"))ImGui::OpenPopup(("ShapeBody##" + std::to_string(GetInstanceID())).c_str());
	if (ImGui::BeginPopup(("ShapeBody##" + std::to_string(GetInstanceID())).c_str()))
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

	ImGui::DragFloat3("OffsetPos", &m_shapeDate.offsetPos.x);
	if (ImGui::Button("ColliderType"))ImGui::OpenPopup(("ColliderType##" + std::to_string(GetInstanceID())).c_str());
	if (ImGui::BeginPopup(("ColliderType##" + std::to_string(GetInstanceID())).c_str()))
	{
		MyImGui::CheckBoxAllBit<KdCollider::Type>(m_shapeDate.tag);
		ImGui::EndPopup();
	}

}

void Cp_Rigidbody::InitJson()
{
	m_move = MyJson::InPutVec3(m_jsonData["move"]);
	m_deceleration = m_jsonData["deceleration"];

	m_bActiveGravity = m_jsonData["activeGravityFlag"];
	m_gravityPow = m_jsonData["gravity"];
	if (m_jsonData["height"].is_number_float())m_height = m_jsonData["height"];
	if (m_jsonData["OffsetHeight"].is_number_float())m_offsetHeight = m_jsonData["OffsetHeight"];

	if (m_jsonData["CollisionBody"].is_boolean())m_collisionBody = m_jsonData["CollisionBody"];
	if (m_jsonData["Shape"].is_number()) m_shape = m_jsonData["Shape"];
	m_shapeDate.radius = MyJson::InPutVec3(m_jsonData["Radius"]);
	m_shapeDate.offsetPos = MyJson::InPutVec3(m_jsonData["OffsetPos"]);
	m_shapeDate.tag = m_jsonData["Tag"];
}
nlohmann::json Cp_Rigidbody::GetJson()
{
	m_jsonData["move"] = MyJson::OutPutVec3(m_move);
	m_jsonData["deceleration"] = m_deceleration;

	m_jsonData["activeGravityFlag"] = m_bActiveGravity;
	m_jsonData["gravity"] = m_gravityPow;
	m_jsonData["height"] = m_height;
	m_jsonData["OffsetHeight"] = m_offsetHeight;


	m_jsonData["CollisionBody"] = m_collisionBody;
	m_jsonData["Shape"] = m_shape;
	m_jsonData["Radius"] = MyJson::OutPutVec3(m_shapeDate.radius);
	m_jsonData["OffsetPos"] = MyJson::OutPutVec3(m_shapeDate.offsetPos);
	m_jsonData["Tag"] = m_shapeDate.tag;
	return m_jsonData;
}

void Cp_Rigidbody::DrawDebug()
{
	if (!m_bActive)return;
	KdDebugWireFrame debugWireFrame;
	if (m_shape == Cp_Rigidbody::Shape::Sphere)
	{
		KdCollider::SphereInfo SphereInfo
		(
			m_shapeDate.tag,
			m_trans.lock()->GetMatrix().Translation() + m_shapeDate.offsetPos,
			m_shapeDate.radius.y
		);
		debugWireFrame.AddDebugSphere(SphereInfo.m_sphere.Center, SphereInfo.m_sphere.Radius);
	}

	else if (m_shape == Cp_Rigidbody::Shape::Box)
	{
		KdCollider::BoxInfo BoxInfo
		(
			m_shapeDate.tag,
			m_trans.lock()->GetMatrix(),
			m_shapeDate.offsetPos,
			m_shapeDate.radius,
			false
		);
		debugWireFrame.AddDebugBox(m_trans.lock()->GetMatrix(), m_shapeDate.radius, m_shapeDate.offsetPos);
	}

	else if (m_shape == Cp_Rigidbody::Shape::Ray)
	{
		KdCollider::RayInfo rayInfo
		(
			m_shapeDate.tag,
			m_trans.lock()->GetMatrix().Translation() + m_shapeDate.offsetPos,
			m_trans.lock()->GetMatrix().Translation() + m_shapeDate.offsetPos + m_move
		);
		debugWireFrame.AddDebugLine(rayInfo.m_pos, rayInfo.m_dir, rayInfo.m_range);
	}

	if (m_gravity)
	{
		Math::Vector3 pos = m_owner.lock()->GetTransform().lock()->GetPosition();
		pos.y -= m_height;
		pos.y += m_offsetHeight;
		KdCollider::RayInfo rayInfo(
			(UINT)KdCollider::Type::TypeGround,
			pos,
			{ 0,abs(m_gravity + m_move.y) / (m_gravity + m_move.y),0 },
			abs(m_gravity + m_move.y) + abs(m_offsetHeight)
		);
		debugWireFrame.AddDebugLine(rayInfo.m_pos, rayInfo.m_dir, rayInfo.m_range);
	}

	debugWireFrame.Draw();
}

float Cp_Rigidbody::Gravity()
{
	if (m_gravity == 0.f)return m_gravity + m_move.y;

	Math::Vector3 pos = m_owner.lock()->GetTransform().lock()->GetPosition();
	pos.y -= m_height;
	pos.y += m_offsetHeight;
	KdCollider::RayInfo rayInfo(
		(UINT)KdCollider::Type::TypeGround,
		pos,
		{ 0,-1,0 },
		abs(m_gravity + m_move.y) + m_offsetHeight
	);

	//重力と床判定
	m_bLanding = false;
	std::list<KdCollider::CollisionResult>	results;
	KdCollider::CollisionResult				effectResult;
	float									hitRange = 0.0f;

	SceneManager::Instance().m_objectMgr->RayHit(rayInfo, &results);
	for (auto& result : results)
	{
		float targetRange = Math::Vector3::Distance(result.m_hitPos, m_trans.lock()->GetPosition());
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

	if (m_shape == Cp_Rigidbody::Shape::Sphere)
	{
		KdCollider::SphereInfo SphereInfo
		(
			m_shapeDate.tag,
			m_trans.lock()->GetMatrix().Translation() + m_shapeDate.offsetPos,
			m_shapeDate.radius.y
		);
		SceneManager::Instance().m_objectMgr->RayHit(SphereInfo, &m_shapeDate.pResults);
	}

	else if (m_shape == Cp_Rigidbody::Shape::Box)
	{
		KdCollider::BoxInfo BoxInfo
		(
			m_shapeDate.tag,
			m_trans.lock()->GetMatrix(),
			m_shapeDate.offsetPos,
			m_shapeDate.radius,
			false
		);
		SceneManager::Instance().m_objectMgr->RayHit(BoxInfo, &m_shapeDate.pResults);
	}

	else if (m_shape == Cp_Rigidbody::Shape::Ray)
	{
		KdCollider::RayInfo rayInfo
		(
			m_shapeDate.tag,
			m_trans.lock()->GetMatrix().Translation() + m_shapeDate.offsetPos,
			m_trans.lock()->GetMatrix().Translation() + m_shapeDate.offsetPos + m_move
		);
		if (rayInfo.m_range)SceneManager::Instance().m_objectMgr->RayHit(rayInfo, &m_shapeDate.pResults);
	}

}
