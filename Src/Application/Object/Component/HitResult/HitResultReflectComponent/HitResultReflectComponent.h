#pragma once
#include "../HitResultComponent.h"

class HitResultReflectComponent
	:public Cp_HitResult
{
public:
	void UpdateContents()override;
	void ImGuiUpdate()override;
	void LoadJson(nlohmann::json _json)override;
	nlohmann::json GetJson()override;

private:
	bool m_bFlying = false;
	bool m_bPlay = false;
	float m_reflectPow = 0.0f;
};
