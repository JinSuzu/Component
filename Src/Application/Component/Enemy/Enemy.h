#pragma once
#include "../Component.h"


class Cp_Enemy 
	:public Component
{
public:
	void Start(std::weak_ptr<Object> _owner)override; 
	void PreUpdate()override;
	void Update()override;
	nlohmann::json GetJson()override;

	void TriggerFn(enum ObjectTag _other);
};