#pragma once
#include "../Component.h"

class Cp_AddRotation
	:public Component
{
public:
	void Start()override; 

	void UpdateContents()override;
	void ImGuiUpdate()override;

	void InitJson()override;
	nlohmann::json GetJson()override;
private:
	enum AddType
	{
		Normal = 1 << 0,
		FollowMouse = 1 << 1
	};

	Math::Vector3 m_addPow;
	AddType m_addType;

	Math::Vector2 m_mouseMove;
};