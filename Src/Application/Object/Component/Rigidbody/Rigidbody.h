#pragma once
#include "../Component.h"

class Cp_Rigidbody 
	:public Component
{
public:
	void Start()override;

	void PreUpdateContents()override;
	void UpdateContents()override;

	void ImGuiUpdate()override;

	void InitJson()override;
	nlohmann::json GetJson()override;

	void AddMove(Math::Vector3 _move) { m_move += _move; }
	Math::Vector3 GetMove()	const	  { return m_move; }
	void SetMove(Math::Vector3 _move) { m_move = _move; }
	void ResetMove()				  { m_move = Math::Vector3::Zero; }

	float GetGravity() const		  { return m_gravityPow; }
	void  SetGravity(float _gravity)  { m_gravity = _gravity; }
	bool GetLanding() const			  { return m_bLanding; }

	float GetHeight() const { return m_height; }
private:	
	float Gravity();

	Math::Vector3 m_move	= {0,0,0};

	bool  m_bActiveGravity  = true;
	bool  m_bLanding		= false;
	float m_gravity			= 0.0f;
	float m_gravityPow		= 1.0f;
	float m_height			= 10.0f;

	float m_deceleration	= 0.98f;

	std::weak_ptr<class Cp_Transform> m_trans;
};