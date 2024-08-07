#pragma once

class RotationLimitComponent
	:public Component 
{
public:
	void PostUpdateContents()override;
	void ImGuiUpdate()override;
	void LoadJson(nlohmann::json _json)override;
	nlohmann::json Serialize()override;
private:
	UINT m_enableShaft = 0;

	bool m_enableMax = false;
	Math::Vector3 m_max;

	bool m_enableMin = false;
	Math::Vector3 m_min;
};