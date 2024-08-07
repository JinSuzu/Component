#pragma once


class PlayerComponent
	:public Component
{
public:
	void Start()override;

	void PreUpdateContents()override;
	void UpdateContents()override;
	void PostUpdateContents()override;

	void ImGuiUpdate()override;
	void LoadJson(nlohmann::json _json)override;
	nlohmann::json Serialize()override;
private:
	std::weak_ptr<class TransformComponent>m_trans;
	std::weak_ptr<class RigidbodyComponent>m_rigid;

	bool  m_jumpFlg = false;
	float m_jumpPow = 1.0f;

	//加速時間
	int m_accelerationTime = 60;
	int m_accelerationTimeCnt = 0;
};
