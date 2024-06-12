#include "Bullet.h"
#include "../../Game/GameObject.h"
#include "../Rigidbody/Rigidbody.h"
#include "../Transform/Transform.h"
#include "../../Game/Manager/GameObjectManager.h"
#include "Bullet.h"

void Cp_Bullet::Start()
{
	m_trans = m_owner.lock()->GetTransform();
	m_rigid = m_owner.lock()->GetComponent<Cp_Rigidbody>();
	m_owner.lock()->DotSave();

	assert(m_rigid.lock() && "リジットボディが無いよ by Bullet");
}

void Cp_Bullet::PreUpdateContents()
{
	Math::Vector3 move = m_trans.lock()->GetRMat().Backward();
	move.Normalize();

	if (m_accelerationTime != m_accelerationTimeCnt)m_accelerationTimeCnt++;
	m_rigid.lock()->AddMove((move * (m_speedPow * m_accelerationTimeCnt / (float)m_accelerationTime)));

	float movingDistance = Math::Vector3::Distance(m_trans.lock()->GetPosition(), m_startPoint);
	if (movingDistance > m_shotRange)m_owner.lock()->Destroy();
}

void Cp_Bullet::ImGuiUpdate()
{
	ImGui::DragFloat3("StartPos", &m_startPos.x);

	ImGui::DragInt("AccelerationTime", &m_accelerationTime);
	ImGui::DragFloat("SpeedPow", &m_speedPow);
	ImGui::DragFloat("ShotRange", &m_shotRange);

	ImGui::InputText("LandingPath", &m_landingPath);
	if (ImGui::SameLine(); ImGui::Button("Input"))
	{
		m_landingObject = GameObjectManager::CreateObject(m_landingPath, m_owner, false);
		m_landingObject->SetActive(false);
		m_landingObject->SetHideFlg(true);
		m_landingObject->DotSave();
	}

	static bool edit = false;
	if (ImGui::Button("LandingObjectEdit"))ImGui::OpenPopup(("LandingObjectEdit##" + std::to_string(m_instanceID)).c_str());
	if (ImGui::BeginPopup(("LandingObjectEdit##" + std::to_string(m_instanceID)).c_str()))
	{
		edit = true;
		GameObjectManager::ImGuiGameObject(m_landingObject);
		ImGui::EndPopup();
	}
	else if (edit)
	{
		m_landingObject->EnableSave();
		m_landingObject->Release();
		m_landingObject->DotSave();
		edit = false;
	}

}

void Cp_Bullet::InitJson()
{
	m_startPos = JsonToVec3(m_jsonData["StartPos"]);

	m_accelerationTime = m_jsonData["AccelerationTime"];
	m_speedPow = m_jsonData["SpeedPow"];
	m_shotRange = m_jsonData["ShotRange"];

	if (m_rigid.lock())Shot();
	if (!m_landingPath.empty())
	{
		m_landingObject = GameObjectManager::CreateObject(m_landingPath, m_owner, false);
		m_landingObject->DotSave();
		m_landingObject->SetActive(false);
	}
}

nlohmann::json Cp_Bullet::GetJson()
{
	m_jsonData["StartPos"] = Vec3ToJson(m_startPos);

	m_jsonData["AccelerationTime"] = m_accelerationTime;
	m_jsonData["SpeedPow"] = m_speedPow;
	m_jsonData["ShotRange"] = m_shotRange;

	m_jsonData["LandingPath"] = m_landingPath;

	return m_jsonData;
}

void Cp_Bullet::Destroy()
{
	Object::Destroy();
	//爆風発生処理
	//ランチャーみてぇな生成仕方にして何でも生成出来るようにする
	if(!m_landingPath.empty())GameObjectManager::CreateObject(m_landingPath, m_owner);
	m_landingObject->DotSave();
}

void Cp_Bullet::Shot()
{
	if (std::weak_ptr<Cp_Transform>pTrans = m_trans.lock()->GetParent().lock(); pTrans.lock())
	{
		m_startPoint = (Math::Matrix::CreateTranslation(m_startPos) * pTrans.lock()->GetMatrix()).Translation();
		m_trans.lock()->SetPosition(m_startPoint);
		Math::Vector3 deg = pTrans.lock()->GetRotation();

		m_trans.lock()->SetRotation(deg);
		m_trans.lock()->DotFollow();

		m_rigid.lock()->ResetMove();
	}
}
