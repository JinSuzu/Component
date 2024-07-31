#include "Rigidbody.h"
#include "../../Game/GameObject.h"
#include "../../Game/Manager/GameObjectManager.h"
#include "../Transform/Transform.h"

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
		Utility::ImGuiHelper::CheckBoxAllBit<KdCollider::Type>(m_shapeDate.tag);
		ImGui::EndPopup();
	}

}

void Cp_Rigidbody::LoadJson(nlohmann::json _json)
{
	m_move = Utility::JsonHelper::InPutVec3(_json["move"]);
	m_deceleration = _json["deceleration"];

	m_bActiveGravity = _json["activeGravityFlag"];
	m_gravityPow = _json["gravity"];
	if (_json["height"].is_number_float())m_height = _json["height"];
	if (_json["OffsetHeight"].is_number_float())m_offsetHeight = _json["OffsetHeight"];

	if (_json["CollisionBody"].is_boolean())m_collisionBody = _json["CollisionBody"];
	if (_json["Shape"].is_number()) m_shape = _json["Shape"];
	m_shapeDate.radius = Utility::JsonHelper::InPutVec3(_json["Radius"]);
	m_shapeDate.offsetPos = Utility::JsonHelper::InPutVec3(_json["OffsetPos"]);
	m_shapeDate.tag = _json["Tag"];
}
nlohmann::json Cp_Rigidbody::GetJson()
{
	nlohmann::json json;
	json["move"] = Utility::JsonHelper::OutPutVec3(m_move);
	json["deceleration"] = m_deceleration;

	json["activeGravityFlag"] = m_bActiveGravity;
	json["gravity"] = m_gravityPow;
	json["height"] = m_height;
	json["OffsetHeight"] = m_offsetHeight;


	json["CollisionBody"] = m_collisionBody;
	json["Shape"] = m_shape;
	json["Radius"] = Utility::JsonHelper::OutPutVec3(m_shapeDate.radius);
	json["OffsetPos"] = Utility::JsonHelper::OutPutVec3(m_shapeDate.offsetPos);
	json["Tag"] = m_shapeDate.tag;
	return json;
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

	GameObjectManager::Instance().RayHit(rayInfo, &results);
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
		GameObjectManager::Instance().RayHit(SphereInfo, &m_shapeDate.pResults);
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
		GameObjectManager::Instance().RayHit(BoxInfo, &m_shapeDate.pResults);
	}

	else if (m_shape == Cp_Rigidbody::Shape::Ray)
	{
		KdCollider::RayInfo rayInfo
		(
			m_shapeDate.tag,
			m_trans.lock()->GetMatrix().Translation() + m_shapeDate.offsetPos,
			m_trans.lock()->GetMatrix().Translation() + m_shapeDate.offsetPos + m_move
		);
		if (rayInfo.m_range)GameObjectManager::Instance().RayHit(rayInfo, &m_shapeDate.pResults);
	}

}
