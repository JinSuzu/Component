#include "ColliderComponent.h"
#include "../../../Application/Object/Game/GameObject.h"
#include "../../../Application/Object/Game/Manager/GameObjectManager.h"

void ColliderComponent::Start()
{
	GameObjectManager::Instance().AddColliderList(WeakThisPtr(this));
	m_trans = m_owner.lock()->GetTransform();
}

void ColliderComponent::UpdateRenderContents()
{
	if (m_colliderShape & ColliderShape::Sphere) 
	{
		RenderManager::Instance().WorkDebugWireFrame().AddDebugSphere(m_trans.lock()->GetMatrix().Translation() + m_offsetPos, m_radius, m_trans.lock()->GetScale());
	}
}

void ColliderComponent::ImGuiUpdate()
{
	bool change = false;
	if (ImGui::Button("ColliderType"))ImGui::OpenPopup(("ColliderType##" + std::to_string(GetInstanceID())).c_str());
	if (ImGui::BeginPopup(("ColliderType##" + std::to_string(GetInstanceID())).c_str()))
	{
		change |= Utility::ImGuiHelper::CheckBoxAllBit<KdCollider::Type>(m_colliderType);
		ImGui::EndPopup();
	}

	if (ImGui::Button("ColliderShape"))ImGui::OpenPopup(("ColliderShape##" + std::to_string(GetInstanceID())).c_str());
	if (ImGui::BeginPopup(("ColliderShape##" + std::to_string(GetInstanceID())).c_str()))
	{
		for (int i = 0; i < magic_enum::enum_count<ColliderShape>(); i++)
		{
			ColliderShape shape = static_cast<ColliderShape>(1 << i);
			change |= ImGui::RadioButton(magic_enum::enum_name(shape).data(), (int*)&m_colliderShape, shape);
		}
		ImGui::EndPopup();
	}

	if (m_colliderShape & ColliderShape::Sphere)
	{
		ImGui::SeparatorText("Sphere");
		change |= ImGui::DragFloat("Radius", &m_radius);
		change |= ImGui::DragFloat3("Offset", &m_offsetPos.x);
	}

	if (change)
	{
		RegisterCollider(true);
		change = false;
	}

}

void ColliderComponent::LoadJson(nlohmann::json _json)
{
	m_colliderShape = _json["ColliderShape"];
	m_colliderType	= _json["ColliderType"];
	m_radius		= _json["Radius"];
	m_offsetPos		= Utility::JsonHelper::InPutVec3(_json["OffsetPos"]);

	RegisterCollider(true);
}

nlohmann::json ColliderComponent::GetJson()
{
	nlohmann::json json;
	json["ColliderShape"] = m_colliderShape;
	json["ColliderType"] = m_colliderType;
	json["Radius"] = m_radius;
	json["OffsetPos"] = Utility::JsonHelper::OutPutVec3(m_offsetPos);

	return json;
}

void ColliderComponent::SetActive(bool _flg)
{
	Object::SetActive(_flg);
	m_collider->SetEnableAll(_flg);
}

bool ColliderComponent::Intersects(const KdCollider::SphereInfo& targetShape, std::list<KdCollider::CollisionResult>* pResults)
{
	if (!m_collider || !m_bActive)return false;
	return m_collider->Intersects(targetShape, m_owner.lock()->GetTransform().lock()->GetMatrix(), pResults);
}
bool ColliderComponent::Intersects(const KdCollider::BoxInfo& targetBox, std::list<KdCollider::CollisionResult>* pResults)
{
	if (!m_collider || !m_bActive)return false;
	return m_collider->Intersects(targetBox, m_owner.lock()->GetTransform().lock()->GetMatrix(), pResults);
}
bool ColliderComponent::Intersects(const KdCollider::RayInfo& targetShape, std::list<KdCollider::CollisionResult>* pResults)
{
	if (!m_collider || !m_bActive)return false;
	return m_collider->Intersects(targetShape, m_owner.lock()->GetTransform().lock()->GetMatrix(), pResults);
}

void ColliderComponent::RegisterCollider(bool _forcePush)
{
	m_collider = std::make_unique<KdCollider>();

	int i = 0;
	if (m_colliderShape & ColliderShape::Asset)
	{
		for (auto& it : m_owner.lock()->GetComponents<ModelDataComponent>())
		{
			if (it.lock()->GetModelData().expired())continue;
			m_collider->RegisterCollisionShape(
				"ObjectCollision" + std::to_string(i++),
				it.lock()->GetModelData().lock(),
				m_colliderType
			);
		}

		for (auto& it : m_owner.lock()->GetComponents<SquarePolygonComponent>())
		{
			if (it.lock()->GetSquarePolygon().expired())continue;
			m_collider->RegisterCollisionShape(
				"ObjectCollision" + std::to_string(i++),
				it.lock()->GetSquarePolygon().lock(),
				m_colliderType
			);
		}
		return;
	}

	if (m_colliderShape & ColliderShape::Cube)
	{
		//未対応
		assert(false && "Cubeコリジョン未開発");
		return;
	}

	if (m_colliderShape & ColliderShape::Sphere)
	{
		m_collider->RegisterCollisionShape(
			"ObjectCollision" + std::to_string(i++),
			m_offsetPos,
			m_radius,
			m_colliderType
		);
	}

}

