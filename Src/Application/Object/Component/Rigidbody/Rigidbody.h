#pragma once
#include "../Component.h"

class Cp_Rigidbody 
	:public Component
{
public:
	void UpdateContents()override;
	void PostUpdateContents()override;
	void ImGuiUpdate()override;

	void InitJson()override;
	nlohmann::json GetJson()override;

	void SetMove(Math::Vector3 _move) { m_move += _move; }
	void SetGravity(float _gravity) { m_gravity += _gravity; }
private:
	Math::Vector3 m_move	= {0,0,0};
	float m_gravity = 0.0f;
	float m_deceleration = 0.98f;
};