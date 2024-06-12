#pragma once
#include "../Component.h"

class Cp_Collider
	:public Component
{
public:
	enum ColliderShape
	{
		Asset = 1 << 0,
		Cube = 1 << 1,
		Sphere = 1 << 2,
		Max = 1 << 3
	};

	void Start()override;
	void ImGuiUpdate()override;
	void InitJson()override;
	nlohmann::json GetJson()override;

	bool Intersects(const KdCollider::SphereInfo& targetShape, std::list<KdCollider::CollisionResult>* pResults);
	bool Intersects(const KdCollider::BoxInfo& targetBox, std::list<KdCollider::CollisionResult>* pResults);
	bool Intersects(const KdCollider::RayInfo& targetShape, std::list<KdCollider::CollisionResult>* pResults);
private:
	void RegisterCollider();

	std::unique_ptr<KdCollider> m_collider;

	UINT m_colliderType;
	UINT m_colliderShape;

	float m_radius = 0.0f;
};
