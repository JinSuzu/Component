#pragma once
#include "../Component.h"

class Cp_Controller
	:public Component
{
public:
	void Start()override;
	void PreUpdateContents()override;
	void ImGuiUpdate()override;
	void InitJson()override;
	nlohmann::json GetJson()override;
private:
	std::weak_ptr<class Cp_Rigidbody> m_rigitbody;

	float m_movePow = 1.0f;
	UINT m_shaft = 0;
};