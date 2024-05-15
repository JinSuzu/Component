#pragma once
#include "../Component.h"

class Cp_Transform;

class Cp_AddRotation
	:public Component
{
public:
	void Start()override; 
	void InitJson()override;

	void UpdateContents()override;
	void ImGuiUpdate()override;

	nlohmann::json GetJson()override;
private:
	Math::Vector3 m_addPow;

	std::weak_ptr<Cp_Transform> m_wpTans;
};