#pragma once
#include "../Component.h"

class Cp_MoveLocus
	:public Component 
{
public:
	void Start()override;
	void PreUpdateContents()override;

	void ImGuiUpdate()override;
	void LoadJson(nlohmann::json _json)override;
	nlohmann::json GetJson()override;
private:
	int m_interval = 5;
	std::shared_ptr<class CreateObject> m_locus;
	std::weak_ptr<class Cp_Rigidbody> m_rigid;
};