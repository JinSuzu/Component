#pragma once
#include "../HitResult.h"

class Cp_HitResultReflect
	:public Cp_HitResult
{
public:
	void UpdateContents()override;
	void ImGuiUpdate()override;
	void InitJson()override;
	nlohmann::json GetJson()override;

private:
	float m_reflectPow = 0.0f;
};
