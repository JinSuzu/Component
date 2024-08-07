#pragma once
#include "../HitResultComponent.h"

class HitResultSceneComponent
	:public Cp_HitResult
{
public:
	void UpdateContents()override;
	void ImGuiUpdate()override;
	void LoadJson(nlohmann::json _json)override;
	nlohmann::json Serialize()override;
private:
	std::string m_nextScene;
};
