#pragma once


class ControllerComponent
	:public Component
{
public:
	void Start()override;
	void PreUpdateContents()override;
	void ImGuiUpdate()override;
	void LoadJson(nlohmann::json _json)override;
	nlohmann::json Serialize()override;
private:
	std::weak_ptr<RigidbodyComponent> m_rigitbody;

	float m_movePow = 1.0f;
	UINT m_shaft = 0;
};