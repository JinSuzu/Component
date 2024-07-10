#include "Bullet.h"
#include "../../Game/GameObject.h"
#include "../Rigidbody/Rigidbody.h"
#include "../Transform/Transform.h"

#include "../../../Utility/CreateObject/CreateObject.h"

void Cp_Bullet::Start()
{
	m_trans = m_owner.lock()->GetTransform();
	m_rigid = m_owner.lock()->GetComponent<Cp_Rigidbody>();
	m_owner.lock()->DotSave();

	assert(m_rigid.lock() && "リジットボディが無いよ by Bullet");

	m_landingObject = std::make_shared<CreateObject>(m_owner);
}

void Cp_Bullet::PreUpdateContents()
{

	Math::Vector3 move = m_trans.lock()->GetRMat().Backward();
	move.Normalize();

	if (m_accelerationTime != m_accelerationTimeCnt)m_accelerationTimeCnt++;
	m_rigid.lock()->AddMove((move * (m_speedPow * m_accelerationTimeCnt / (float)m_accelerationTime)));

	if (m_startPoint == Math::Vector3::Zero)m_startPoint = m_trans.lock()->GetPosition();
	float movingDistance = Math::Vector3::Distance(m_trans.lock()->GetPosition(), m_startPoint);
	if (movingDistance > m_shotRange)m_owner.lock()->Destroy();
}

void Cp_Bullet::UpdateContents()
{
	bool flg = false;
	float range = 0.0f;
	for (auto& result : m_rigid.lock()->GetHitResult())
	{
		if (range > result.m_overlapDistance || !flg)
		{
			range = result.m_overlapDistance;
			m_trans.lock()->SetPosition(result.m_hitPos);
			flg = true;
		}
	}
	if (flg)m_owner.lock()->Destroy();
}

void Cp_Bullet::ImGuiUpdate()
{
	ImGui::DragInt("AccelerationTime", &m_accelerationTime);
	ImGui::DragFloat("SpeedPow", &m_speedPow);
	ImGui::DragFloat("ShotRange", &m_shotRange);

	m_landingObject->ImGuiUpdate();
}

void Cp_Bullet::LoadJson(nlohmann::json _json)
{
	m_accelerationTime = _json["AccelerationTime"];
	m_speedPow		   = _json["SpeedPow"];
	m_shotRange		   = _json["ShotRange"];

	m_landingObject->SetJson(_json);
}

nlohmann::json Cp_Bullet::GetJson()
{
	nlohmann::json json;
	json["AccelerationTime"] = m_accelerationTime;
	json["SpeedPow"] = m_speedPow;
	json["ShotRange"] = m_shotRange;
	m_landingObject->OutJson(json);

	return json;
}

void Cp_Bullet::Destroy()
{
	Object::Destroy();
	KdAudioManager::Instance().Play("Asset/Sounds/Explosion.wav");
	//爆風発生処理
	m_landingObject->Create();
	m_owner.lock()->GetChilds().clear();
}