﻿#pragma once


class TransFormInheritComponent
	:public Component
{
public:
	void Start()override;
	void ImGuiUpdate()override;
	void LoadJson(nlohmann::json _json)override;
	nlohmann::json Serialize()override;

private:
	bool m_formInheritPos = false;
	UINT m_activePos = 0;
	Math::Vector3 m_startPos = Math::Vector3::Zero;

	bool m_formInheritRota = false;
	bool m_cameraFormInherit = false;
	UINT m_activeRota = 0;
	Math::Vector3 m_startRota = Math::Vector3::Zero;

	bool m_formInheritScale = false;
	UINT m_activeScale = 0;
	Math::Vector3 m_startScale = Math::Vector3::One;
};
