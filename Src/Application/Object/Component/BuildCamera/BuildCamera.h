﻿#pragma once
#include "../Component.h"

class Cp_BuildCamera
	:public Component
{
public:
	void Start()override;
	void ImGuiUpdate()override;

	void HoverMove();
	void AngleShift();

private:
	Math::Vector3 m_speedPow = { 1,1,1 };
};
