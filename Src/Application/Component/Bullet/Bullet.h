#pragma once
#include "../Component.h"

class Object;
class Cp_Transform;
class Cp_BoxCollision;

class Cp_Bullet
	:public Component
{
public:
	void Start(std::weak_ptr<Object> _owner)override; 
	void PostUpdate()override;
	void Update()override;

	void Emit(std::weak_ptr<Object> _obj,float _angl);
	nlohmann::json GetJson()override;

	void TriggerFn(enum ObjectTag _tag);
private:
	std::weak_ptr<Cp_Transform>m_swTrans;
	std::weak_ptr<Cp_BoxCollision>m_swCollison;

	Math::Vector3 move;
	float speedPow = 5.0f;
};