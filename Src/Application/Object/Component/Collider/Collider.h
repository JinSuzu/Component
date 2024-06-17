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

	void SetActive(bool _flg)override;

	bool Intersects(const KdCollider::SphereInfo& targetShape, std::list<KdCollider::CollisionResult>* pResults);
	bool Intersects(const KdCollider::BoxInfo& targetBox, std::list<KdCollider::CollisionResult>* pResults);
	bool Intersects(const KdCollider::RayInfo& targetShape, std::list<KdCollider::CollisionResult>* pResults);

	void DrawDebug();
private:
	void RegisterCollider(bool _forcePush = false);

	std::unique_ptr<KdCollider> m_collider;
	std::weak_ptr<class Cp_Transform> m_trans;

	UINT m_colliderType		= 0;
	UINT m_colliderShape	= 0;

	float m_radius			= 0.0f;

	Math::Vector3 m_offsetPos;
	std::shared_ptr<std::function<void()>>m_drawDebug;
};
