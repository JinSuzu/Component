#pragma once
#include "../Component.h"

class Cp_Transform;

class Cp_PlayerCircle
	:public Component 
{
public:
	void Start(std::weak_ptr<Object> _owner)override;
	void Update()override; 
	nlohmann::json GetJson()override;
private:
	std::weak_ptr<Cp_Transform>m_wpTrans;
	
};