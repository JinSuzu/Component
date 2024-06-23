#pragma once
#include "../Component.h"

class Cp_TransFormInherit
	:public Component
{
public:
	void Start()override;
	void ImGuiUpdate()override;
	void InitJson()override;
	nlohmann::json GetJson()override;

private:
	bool m_formInheritPos = false;
	UINT m_activePos = 0;
	Math::Vector3 m_startPos = Math::Vector3::Zero;

	bool m_formInheritRota = false;
	UINT m_activeRota = 0;
	Math::Vector3 m_startRota = Math::Vector3::Zero;

	bool m_formInheritScale = false;
	UINT m_activeScale = 0;
	Math::Vector3 m_startScale = Math::Vector3::One;
};
