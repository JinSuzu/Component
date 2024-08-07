#pragma once

class FindChildComponent
	:public Component
{
public:
	void PreUpdateContents()override;

	const std::weak_ptr<GameObject>& GetHoldChild() {return m_holdChild; }
	std::weak_ptr<GameObject>& WorkHoldChild() { return m_holdChild; }

	void ImGuiUpdate()override;
	nlohmann::json Serialize()override;
	void LoadJson(nlohmann::json _json)override;

private:
	std::string m_searchName;
	std::weak_ptr<GameObject> m_holdChild;
};
