#pragma once
#include "../Component.h"

class Cp_Rigidbody 
	:public Component
{
public:
	void Start()override;

	void UpdateContents()override;

	void ImGuiUpdate()override;

	void InitJson()override;
	nlohmann::json GetJson()override;

	void SetMove(Math::Vector3 _move) { m_move += _move; }
	void ResetMove()				  { m_move = Math::Vector3::Zero; }
	void SetGravity(float _gravity)   { m_gravity += _gravity; }
private:
	float Gravity();

	Math::Vector3 m_move	= {0,0,0};

	bool  m_bActiveGravity  = true;
	float m_gravity			= 0.0f;
	float m_gravityPow		= 0.0f;
	float m_height			= 10.0f;

	float m_deceleration	= 0.98f;

	std::weak_ptr<class Cp_Transform> m_trans;
};