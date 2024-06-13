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

	void UpdateContents()override;

	void ImGuiUpdate()override;
	void InitJson()override;
	nlohmann::json GetJson()override;

	bool Intersects(const KdCollider::SphereInfo& targetShape, std::list<KdCollider::CollisionResult>* pResults);
	bool Intersects(const KdCollider::BoxInfo& targetBox, std::list<KdCollider::CollisionResult>* pResults);
	bool Intersects(const KdCollider::RayInfo& targetShape, std::list<KdCollider::CollisionResult>* pResults);

	void DrawDebug();
private:
	void RegisterCollider();

	std::unique_ptr<KdCollider> m_collider;

	UINT m_colliderType		= 0;
	UINT m_colliderShape	= 0;

	float m_radius			= 0.0f;
	
	std::shared_ptr<std::function<void()>> m_drawDebug;
	std::shared_ptr<KdDebugWireFrame>m_debugWireFrame;
};
