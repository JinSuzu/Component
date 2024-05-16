#include "SceneBase.h"
#include "../Object/Game/Manager/GameObjectManager.h"

void SceneBase::Draw2D()
{
	GameObjectManager::Instance().Draw();
}

void SceneBase::PreUpdate()
{
	GameObjectManager::Instance().PreUpdate();
}

void SceneBase::Update()
{
	GameObjectManager::Instance().Update();
}

void SceneBase::PostUpdate()
{
	GameObjectManager::Instance().PostUpdate();
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
	GameObjectManager::Instance().Release();
	ReleaseContent();
}

void SceneBase::ImGuiUpdate()
{
	ImGui::Text(id.c_str());
}
