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
	enum RotationShaft 
	{
		X = 1 << 0,
		Y = 1 << 1,
		Z = 1 << 2
	};

	std::weak_ptr<class Cp_Rigidbody> m_rigitbody;

	float m_movePow = 1.0f;
	unsigned int m_shaft = {
		RotationShaft::X |
		RotationShaft::Y |
		RotationShaft::Z
	};
};