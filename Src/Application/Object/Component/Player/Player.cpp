﻿#include "Player.h"
#include "../../Game/GameObject.h"
#include "../Rigidbody/Rigidbody.h"

void Cp_Player::Start()
{
	m_rigid = m_owner.lock()->GetComponent<Cp_Rigidbody>();
	assert(m_rigid.lock()&&"リジットないよbyPlayer");
}

void Cp_Player::PreUpdateContents()
{
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		if (!m_jumpFlg)
		{
			m_jumpFlg = true;
			m_accelerationTimeCnt = 0;
			m_rigid.lock()->SetGravity(0);
		}
	}

	if (m_jumpFlg)
	{
		if (m_accelerationTimeCnt != m_accelerationTime) 
		{
			m_rigid.lock()->AddMove(Math::Vector3(0, m_jumpPow * (m_accelerationTime / (float)(m_accelerationTimeCnt + 1)), 0));
			m_accelerationTimeCnt++;
		}
	}
}

void Cp_Player::PostUpdateContents()
{
	if (m_jumpFlg) 
	{
		m_jumpFlg = !m_rigid.lock()->GetLanding();
	}
}

void Cp_Player::ImGuiUpdate()
{
	ImGui::DragFloat("JumpPow",&m_jumpPow);
	ImGui::DragInt("AccelerationTime", &m_accelerationTime);
}

void Cp_Player::InitJson()
{
	m_jumpPow = m_jsonData["jumpFlg"];
	m_accelerationTime = m_jsonData["AccelerationTime"];
}

nlohmann::json Cp_Player::GetJson()
{
	m_jsonData["jumpFlg"] = m_jumpPow;
	m_jsonData["AccelerationTime"] = m_accelerationTime;
	return m_jsonData;
}
