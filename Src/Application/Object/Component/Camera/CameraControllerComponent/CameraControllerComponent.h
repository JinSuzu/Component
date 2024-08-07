#pragma once

class CameraControllerComponent :public Component
{
public:
	void PreUpdateContents()override;

	void ImGuiUpdate()override;
	nlohmann::json Serialize()override;
	void LoadJson(nlohmann::json _json)override;

private:
	UINT m_effectShaft = (UINT)Shaft::X;
};
