﻿#include "Collider.h"
#include "../../Game/GameObject.h"
#include "../../Game/Manager/GameObjectManager.h"
#include "../../../SceneBase/Manager/SceneManager.h"
#include "../../../SceneBase/SceneBase.h"

#include "../Transform/Transform.h"
#include "../ModelData/ModelData.h"
#include "../SquarePolygon/SquarePolygon.h"

void Cp_Collider::Start()
{
	m_collider = std::make_unique<KdCollider>();
	RegisterCollider();

	SceneManager::Instance().GetNowScene().lock()
		->GetGameObject().AddColliderList(WeakThisPtr(this));
}

void Cp_Collider::ImGuiUpdate()
{
	int i = 0;
	bool chenge = false;
	if (ImGui::Button("ColliderType"))ImGui::OpenPopup(("ColliderType##" + std::to_string(m_instanceID)).c_str());
	if (ImGui::BeginPopup(("ColliderType##" + std::to_string(m_instanceID)).c_str()))
	{
		for (int i = 0; i < std::log2((double)KdCollider::Type::TypeMax); i++)
		{
			KdCollider::Type type = static_cast<KdCollider::Type>(1 << i);
			chenge |= ImGuiCheckBoxBit(magic_enum::enum_name(type).data(), m_colliderType, type);
		}
		ImGui::EndPopup();
	}

	if (ImGui::Button("ColliderShape"))ImGui::OpenPopup(("ColliderShape##" + std::to_string(m_instanceID)).c_str());
	if (ImGui::BeginPopup(("ColliderShape##" + std::to_string(m_instanceID)).c_str()))
	{
		for (int i = 0; i < std::log2((double)ColliderShape::Max); i++)
		{
			ColliderShape shape = static_cast<ColliderShape>(1 << i);
			ImGui::RadioButton(magic_enum::enum_name(shape).data(), (int*)&m_colliderShape, shape);
		}
		ImGui::EndPopup();
	}

	if (m_colliderShape & ColliderShape::Sphere)
	{
		ImGui::SeparatorText("Sphere");
		ImGui::DragFloat("Radius", &m_radius);
	}

	if (chenge)
	{
		RegisterCollider();
	}
}

void Cp_Collider::InitJson()
{
	m_colliderShape = m_jsonData["ColliderShape"];
	m_colliderType = m_jsonData["ColliderType"];

	m_radius = m_jsonData["Radius"];
}

nlohmann::json Cp_Collider::GetJson()
{
	m_jsonData["ColliderShape"] = m_colliderShape;
	m_jsonData["ColliderType"] = m_colliderType;

	m_jsonData["Radius"] = m_radius;
	return m_jsonData;
}

bool Cp_Collider::Intersects(const KdCollider::SphereInfo& targetShape, std::list<KdCollider::CollisionResult>* pResults)
{
	return m_collider->Intersects(targetShape, m_owner.lock()->GetTransform().lock()->GetMatrix(), pResults);
}
bool Cp_Collider::Intersects(const KdCollider::BoxInfo& targetBox, std::list<KdCollider::CollisionResult>* pResults)
{
	return m_collider->Intersects(targetBox, m_owner.lock()->GetTransform().lock()->GetMatrix(), pResults);
}
bool Cp_Collider::Intersects(const KdCollider::RayInfo& targetShape, std::list<KdCollider::CollisionResult>* pResults)
{
	return m_collider->Intersects(targetShape, m_owner.lock()->GetTransform().lock()->GetMatrix(), pResults);
}

void Cp_Collider::RegisterCollider()
{
	int i = 0;
	if (m_colliderType & ColliderShape::Asset)
	{
		for (auto& it : m_owner.lock()->GetComponents<Cp_ModelData>())
		{
			m_collider->RegisterCollisionShape(
				"ObjectCollition" + std::to_string(i++),
				it.lock()->GetModelData().lock(),
				m_colliderType
			);
		}

		for (auto& it : m_owner.lock()->GetComponents<Cp_SquarePolygon>())
		{
			m_collider->RegisterCollisionShape(
				"ObjectCollition" + std::to_string(i++),
				it.lock()->GetSquarePolygon().lock(),
				m_colliderType
			);
		}
		return;
	}

	if (m_colliderType & ColliderShape::Cube)
	{
		//未対応
		assert(false && "Cubeコリジョン未開発");
		return;
	}
	if (m_colliderType & ColliderShape::Sphere)
	{
		m_collider->RegisterCollisionShape(
			"ObjectCollition" + std::to_string(i++),
			m_owner.lock()->GetTransform().lock()->GetPosition(),
			m_radius,
			m_colliderType
		);
		return;
	}

}

