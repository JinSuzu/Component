#include "Collider.h"
#include "../../Game/GameObject.h"
#include "Collider.h"

#include "../Transform/Transform.h"
#include "../ModelData/ModelData.h"
#include "../SquarePolygon/SquarePolygon.h"

void Cp_Collider::Start()
{
	int i = 0;
	for (auto& it : m_owner.lock()->GetComponents<Cp_ModelData>()) 
	{
		m_collider.RegisterCollisionShape(
			"Object" + std::to_string(i),
			it.lock()->GetModelData().lock(),
			KdCollider::Type::TypeGround
		);
	}
	
	
}

bool Cp_Collider::Intersects(const KdCollider::SphereInfo& targetShape, std::list<KdCollider::CollisionResult>* pResults)
{
	return m_collider.Intersects(targetShape, m_owner.lock()->GetTransform().lock()->GetMatrix(), pResults);
}
bool Cp_Collider::Intersects(const KdCollider::BoxInfo& targetBox, std::list<KdCollider::CollisionResult>* pResults)
{
	return m_collider.Intersects(targetBox, m_owner.lock()->GetTransform().lock()->GetMatrix(), pResults);
}
bool Cp_Collider::Intersects(const KdCollider::RayInfo& targetShape, std::list<KdCollider::CollisionResult>* pResults)
{
	return m_collider.Intersects(targetShape, m_owner.lock()->GetTransform().lock()->GetMatrix(), pResults);
}

