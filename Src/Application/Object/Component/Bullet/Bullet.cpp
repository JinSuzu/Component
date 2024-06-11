#include "Bullet.h"
#include "../../Game/GameObject.h"
#include "../Rigidbody/Rigidbody.h"
#include "../Transform/Transform.h"
#include "../../../Utility/Timer.h"
#include "Bullet.h"

void Cp_Bullet::Start()
{
	m_trans = m_owner.lock()->GetTransform();
	m_rigid = m_owner.lock()->GetComponent<Cp_Rigidbody>();
	m_owner.lock()->DotSave();
	m_trans.lock()->DotFollow();

	if (m_rigid.lock())Shot();
	else assert(m_rigid.lock() && "リジットボディが無いよ by Bullet");
}

void Cp_Bullet::PreUpdateContents()
{
	Math::Vector3 move = m_trans.lock()->GetRMat().Backward();
	move.Normalize();

	if (m_final != m_phase)m_phase++;
	m_rigid.lock()->SetMove((move * (m_speedPow * m_phase / (float)m_final)));
}

void Cp_Bullet::ImGuiUpdate()
{
	ImGui::DragInt("SpeedUpSection",&m_final);
	ImGui::DragFloat("SpeedPow",&m_speedPow);
}

void Cp_Bullet::InitJson()
{
	m_final = m_jsonData["SpeedUpSection"];
	m_speedPow = m_jsonData["SpeedPow"];
}

nlohmann::json Cp_Bullet::GetJson()
{
	m_jsonData["SpeedUpSection"] = m_final;
	m_jsonData["SpeedPow"] = m_speedPow;

	return m_jsonData;
}

void Cp_Bullet::Shot()
{
	if (m_owner.lock()->GetParent().expired())return;
	std::weak_ptr<Cp_Transform>parentTrans = m_owner.lock()->GetParent().lock()->GetTransform();

	Math::Vector3 pos = (Math::Matrix::CreateTranslation(0, 0, 0.43) * parentTrans.lock()->GetMatrix()).Translation();
	m_trans.lock()->SetPosition(pos);
	Math::Vector3 deg = parentTrans.lock()->GetRotation();
	m_trans.lock()->SetRotation(deg);

	m_rigid.lock()->ResetMove();
}
