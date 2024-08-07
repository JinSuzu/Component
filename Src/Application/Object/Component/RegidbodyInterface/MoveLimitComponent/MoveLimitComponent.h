#pragma once




class MoveLimitComponent
	:public Component
{
public:
	void Start()override;
	void PreUpdateContents()override;
	void ImGuiUpdate()override;
	void LoadJson(nlohmann::json _json)override;
	nlohmann::json Serialize()override;

private:
	//射程
	float m_shotRange = 10;
	Math::Vector3 m_startPoint;
	std::weak_ptr<class TransformComponent> m_trans;
};
