﻿#pragma once

class AddRotationComponent
	:public Component
{
public:
	void Start()override; 

	void UpdateContents()override;
	void ImGuiUpdate()override;

	void LoadJson(nlohmann::json _json)override;
	nlohmann::json Serialize()override;

	void SetActive(bool _flg);
private:
	enum AddType
	{
		Normal = 1 << 0,
		FollowMouse = 1 << 1,
	};

	Math::Vector3 m_addPow;
	AddType m_addType = AddType::Normal;

	Math::Vector2 m_mouseMove;
};