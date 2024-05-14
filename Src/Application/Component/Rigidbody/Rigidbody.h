#pragma once
#include "../Component.h"

class Cp_Rigidbody 
	:public Component
{
public:
	void Start()override; 
	void InitJson()override;

	void PreUpdate()override;
	void Update()override;
	//void PostUpdate()override;
	void ImGuiUpdate()override;

	nlohmann::json GetJson()override;

	void SetMove(Math::Vector3 _move) { m_move = _move; }
	void SetGravity(Math::Vector3 _gravity) { m_gravity = _gravity; }
private:
	Math::Vector3 m_move;
	Math::Vector3 m_gravity;
};


GENCOMPONENTLIST(Rigidbody)