#pragma once
#include "../Component.h"

class Cp_TransFormInherit
	:public Component
{
public:
	void ImGuiUpdate()override;
	void InitJson()override;
	nlohmann::json GetJson()override;

private:
	bool m_formInheritPos = false;
	Math::Vector3 m_startPos = Math::Vector3::Zero;
	bool m_formInheritRota = false;
	Math::Vector3 m_startRota = Math::Vector3::Zero;
	bool m_formInheritScale = false;
	Math::Vector3 m_startScale = Math::Vector3::One;
};
