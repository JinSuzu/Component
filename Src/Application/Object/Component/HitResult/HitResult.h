#pragma once
#include "../Component.h"

class Cp_HitResult
	:public Component
{
public:
	void Start()override;
	void ImGuiUpdate()override;
	void LoadJson(nlohmann::json _json)override;
	nlohmann::json GetJson()override;

protected:
	std::weak_ptr<class Cp_Transform> m_trans;
	std::weak_ptr<class Cp_Rigidbody> m_rigid;

	UINT m_colType = (UINT)KdCollider::Type::TypeBound;
};
