#include "Collider.h"
#include "../../Game/GameObject.h"
#include "../../Game/Manager/GameObjectManager.h"
#include "../../../SceneBase/Manager/SceneManager.h"
#include "../../../SceneBase/SceneBase.h"

#include "../Transform/Transform.h"
#include "../ModelData/ModelData.h"
#include "../SquarePolygon/SquarePolygon.h"

#include "../../../RenderManger/RenderManger.h"

void Cp_Collider::Start()
{
	SceneManager::Instance().m_objectMgr->AddColliderList(WeakThisPtr(this));
	m_trans = m_owner.lock()->GetTransform();

	m_drawDebug = std::make_shared<std::function<void()>>([this]() {DrawDebug(); });
	RenderManager::Instance().AddDrawDebug(m_drawDebug);
}

void Cp_Collider::ImGuiUpdate()
{
	bool change = false;
	if (ImGui::Button("ColliderType"))ImGui::OpenPopup(("ColliderType##" + std::to_string(GetInstanceID())).c_str());
	if (ImGui::BeginPopup(("ColliderType##" + std::to_string(GetInstanceID())).c_str()))
	{
		for (int i = 0; i < std::log2((double)KdCollider::Type::TypeMax); i++)
		{
			KdCollider::Type type = static_cast<KdCollider::Type>(1 << i);
			change |= ImGuiCheckBoxBit(magic_enum::enum_name(type).data(), m_colliderType, (UINT)type);
		}
		ImGui::EndPopup();
	}

	if (ImGui::Button("ColliderShape"))ImGui::OpenPopup(("ColliderShape##" + std::to_string(GetInstanceID())).c_str());
	if (ImGui::BeginPopup(("ColliderShape##" + std::to_string(GetInstanceID())).c_str()))
	{
		for (int i = 0; i < std::log2((double)ColliderShape::Max); i++)
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

void Cp_Collider::InitJson()
{
	m_colliderShape = m_jsonData["ColliderShape"];
	m_colliderType = m_jsonData["ColliderType"];
	m_radius = m_jsonData["Radius"];
	m_offsetPos = JsonToVec3(m_jsonData["OffsetPos"]);

	RegisterCollider(true);
}

nlohmann::json Cp_Collider::GetJson()
{
	m_jsonData["ColliderShape"] = m_colliderShape;
	m_jsonData["ColliderType"] = m_colliderType;
	m_jsonData["Radius"] = m_radius;
	m_jsonData["OffsetPos"] = Vec3ToJson(m_offsetPos);

	return m_jsonData;
}

void Cp_Collider::SetActive(bool _flg)
{
	Object::SetActive(_flg);
	m_collider->SetEnableAll(_flg);
}

bool Cp_Collider::Intersects(const KdCollider::SphereInfo& targetShape, std::list<KdCollider::CollisionResult>* pResults)
{
	if (!m_collider || !m_bActive)return false;
	return m_collider->Intersects(targetShape, m_owner.lock()->GetTransform().lock()->GetMatrix(), pResults);
}
bool Cp_Collider::Intersects(const KdCollider::BoxInfo& targetBox, std::list<KdCollider::CollisionResult>* pResults)
{
	if (!m_collider || !m_bActive)return false;
	return m_collider->Intersects(targetBox, m_owner.lock()->GetTransform().lock()->GetMatrix(), pResults);
}
bool Cp_Collider::Intersects(const KdCollider::RayInfo& targetShape, std::list<KdCollider::CollisionResult>* pResults)
{
	if (!m_collider || !m_bActive)return false;
	return m_collider->Intersects(targetShape, m_owner.lock()->GetTransform().lock()->GetMatrix(), pResults);
}

void Cp_Collider::DrawDebug()
{
	if (!m_bActive)return;
	KdDebugWireFrame debugWireFrame;
	if (m_colliderShape & ColliderShape::Sphere)debugWireFrame.AddDebugSphere(m_trans.lock()->GetPosition() + m_offsetPos, m_radius, m_trans.lock()->GetScale());
	debugWireFrame.Draw();
}

void Cp_Collider::RegisterCollider(bool _forcePush)
{
	m_collider = std::make_unique<KdCollider>();

	int i = 0;
	if (m_colliderShape & ColliderShape::Asset)
	{
		for (auto& it : m_owner.lock()->GetComponents<Cp_ModelData>())
		{
			if (it.lock()->GetModelData().expired())continue;
			m_collider->RegisterCollisionShape(
				"ObjectCollision" + std::to_string(i++),
				it.lock()->GetModelData().lock(),
				m_colliderType
			);
		}

		for (auto& it : m_owner.lock()->GetComponents<Cp_SquarePolygon>())
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

