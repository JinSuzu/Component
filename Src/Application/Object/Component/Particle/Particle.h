#pragma once
#pragma once
#include "../Component.h"

class Cp_Particle
	:public Component
{
public:
	void Start()override;
	void PreUpdateContents()override;
	void ImGuiUpdate()override;
	void LoadJson(nlohmann::json _json)override;
	nlohmann::json GetJson()override;

private:
	int m_deleteTimeCnt = 0;
	int m_deleteTime    = 0;
};
