#include "SceneBase.h"
#include "../Object/Game/Manager/GameObjectManager.h"

void SceneBase::PreUpdate()
{
	m_objectMgr.PreUpdate();
}

void SceneBase::Update()
{
	m_objectMgr.Update();
}

void SceneBase::PostUpdate()
{
	m_objectMgr.PostUpdate();
}

void SceneBase::Load()
{
	if (m_bLoad)return;//ロード済みreturn
	m_bLoad = true;
	m_objectMgr.Load("Scene/" + m_name);
	LoadContent();
}

void SceneBase::Release()
{
	if (!m_bLoad)return;//リリース済みreturn
	m_bLoad = false;
	m_objectMgr.Release("Scene/" + m_name);
	ReleaseContent();
}

void SceneBase::ImGuiUpdate()
{
	ImGui::Text(m_name.c_str());
}
