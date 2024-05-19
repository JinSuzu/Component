#include "SceneBase.h"
#include "../Object/Game/Manager/GameObjectManager.h"

void SceneBase::Draw2D()
{
	m_objectMgr.Draw();
}

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
	LoadContent();
	std::ifstream file(id);
	if (file.is_open()) {
		file >> m_jsonData;
	}
}

void SceneBase::Release()
{
	if (!m_bLoad)return;//リリース済みreturn
	m_bLoad = false;
	m_objectMgr.Release();
	ReleaseContent();
}

void SceneBase::ImGuiUpdate()
{
	ImGui::Text(id.c_str());
}
