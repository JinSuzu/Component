#pragma once
#include "../HitResult.h"

class Cp_HitResultScene
	:public Cp_HitResult
{
public:
	void UpdateContents()override;
	void ImGuiUpdate()override;
	void LoadJson(nlohmann::json _json)override;
	nlohmann::json GetJson()override;
private:
	std::string m_nextScene;
};
