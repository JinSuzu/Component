#pragma once

class CreateObjectComponent
	:public Component
{
public:
	void Start()override;
	
	void Create();

	void ImGuiUpdate()override;

	nlohmann::json Serialize()override;
	void LoadJson(nlohmann::json _json)override;

private:
	std::string			  m_dataPath;

	std::shared_ptr<class GameObject> m_object;
	bool m_edited = false;
};
