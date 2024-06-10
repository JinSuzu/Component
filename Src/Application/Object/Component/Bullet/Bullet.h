#pragma once
#include "../Component.h"

class Cp_Bullet
	:public Component
{
public:
	void Start()override;
	void PreUpdateContents()override;
	void ImGuiUpdate()override;
	void InitJson()override;
	nlohmann::json GetJson()override;

private:
	int m_final = 60;
	int m_phase = 0;

	float m_speedPow = 0.01f;
	std::weak_ptr<class Cp_Rigidbody>m_rigid;
	std::weak_ptr<class Cp_Transform>m_trans;
};
