#include "CreateObject.h"
#include "../../Object/Game/Manager/GameObjectManager.h"
#include "../../Object/Game/GameObject.h"

std::shared_ptr<GameObject> CreateObject::Create()
{
	std::shared_ptr<GameObject> obj = GameObjectManager::CreateObject(m_createSet.path, m_parentObject);
	obj->DotSave();
	return obj;
}
void CreateObject::ImGuiUpdate()
{
	ImGui::InputText("ObjectPath",&m_createSet.path);
	if (ImGui::SameLine(); ImGui::Button("Input"))
	{
		m_createSet.object = GameObjectManager::CreateObject(m_createSet.path, m_parentObject, false);
		m_createSet.object->SetActive(false);
		m_createSet.object->SetHideFlg(true);
		m_createSet.object->DotSave();
	}

	static bool edit = false;
	if (ImGui::Button("ObjectEdit"))ImGui::OpenPopup(("ObjectEdit##" + std::to_string(m_id)).c_str());
	if (!m_createSet.object)return;
	if (ImGui::BeginPopup(("ObjectEdit##" + std::to_string(m_id)).c_str()))
	{
		edit = true;
		GameObjectManager::ImGuiGameObject(m_createSet.object);
		ImGui::EndPopup();
	}
	else if (edit)
	{
		m_createSet.object->EnableSave();
		m_createSet.object->Release();
		m_createSet.object->DotSave();
		edit = false;
	}
}

void CreateObject::OutJson(nlohmann::json& _json)
{
	_json["CreatePath"] = m_createSet.path;
}

void CreateObject::SetJson(nlohmann::json _json)
{
	if (_json["CreatePath"].is_string())m_createSet.path = _json["CreatePath"];
	if (!m_createSet.path.empty()) 
	{
		m_createSet.object = GameObjectManager::CreateObject(m_createSet.path, m_parentObject, false);
		m_createSet.object->SetActive(false);
		m_createSet.object->SetHideFlg(true);
		m_createSet.object->DotSave();
	}
}
