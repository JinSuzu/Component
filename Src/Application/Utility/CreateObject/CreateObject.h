#pragma once

class GameObject;
class CreateObject
{
	struct CreateSet
	{
		std::string path;
		std::shared_ptr<GameObject> object;
	};
public:
	CreateObject(std::weak_ptr<GameObject> _obj)
		:m_parentObject(_obj)
		, m_id((int)(this))
	{}

	std::shared_ptr<GameObject> Create();

	void ImGuiUpdate();

	void OutJson(nlohmann::json& _json);
	void SetJson(nlohmann::json _json);
private:
	CreateSet m_createSet;
	std::weak_ptr<GameObject> m_parentObject;

	UINT m_id;
};
