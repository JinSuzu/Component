#include "SceneBase.h"
#include "../Object/Manager/ObjectManager.h"

void SceneBase::Draw2D()
{
	ObjectManager::Instance().Draw();
}

void SceneBase::PreUpdate()
{
	ObjectManager::Instance().PreUpdate();
}

void SceneBase::Update()
{
	ObjectManager::Instance().Update();
}

void SceneBase::PostUpdate()
{
	ObjectManager::Instance().PostUpdate();
}

void SceneBase::Load()
{
	if (m_bLoad)return;//ロード済みreturn
	m_bLoad = true;
	LoadContent();
}

void SceneBase::Release()
{
	if (!m_bLoad)return;//リリース済みreturn
	m_bLoad = false;
	ObjectManager::Instance().Release();
	ReleaseContent();
}

void SceneBase::ImGuiUpdate()
{
	ImGui::Text(id.c_str());
}
