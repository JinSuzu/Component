#pragma once


class BulletComponent
	:public Component
{
public:
	void Start()override;
	void PreUpdateContents()override;
	void UpdateContents()override;
	void ImGuiUpdate()override;
	void LoadJson(nlohmann::json _json)override;
	nlohmann::json Serialize()override;

	void Destroy()override;

	
private:
	//加速時間
	int m_accelerationTime = 60;
	int m_accelerationTimeCnt = 0;

	//射程
	float m_shotRange = 10;
	Math::Vector3 m_startPoint;

	//着弾処理
	float m_radius = 0.0f;
	std::shared_ptr<class CreateObject> m_landingObject;

	float m_speedPow = 0.1f;
	std::weak_ptr<class RigidbodyComponent>m_rigid;
	std::weak_ptr<class TransformComponent>m_trans;
};
