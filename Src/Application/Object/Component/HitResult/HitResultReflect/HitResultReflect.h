#pragma once
#include "../HitResult.h"

class Cp_HitResultReflect
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
