#pragma once

class Cp_HitResult
	:public Component
{
public:
	void Start()override;
	void ImGuiUpdate()override;
	void LoadJson(nlohmann::json _json)override;
	nlohmann::json GetJson()override;

protected:
	std::weak_ptr<TransformComponent> m_trans;
	std::weak_ptr<RigidbodyComponent> m_rigid;

	UINT m_colType = (UINT)KdCollider::Type::TypeBound;
};
