#include "CreateObjectComponent.h"
#include "../../Game/GameObject.h"
#include "../../Game/Manager/GameObjectManager.h"
#include "../../../main.h"

#include "../../../../System/EditorWindow/Prefab/Prefab.h"

void CreateObjectComponent::Start()
{
}

void CreateObjectComponent::Create()
{
	if (!m_dataPath.empty())
	{
		GameObjectManager::Instance().CreateObject(m_dataPath, m_owner);
	}
}

void CreateObjectComponent::ImGuiUpdate()
{
	if (ImGui::Button(m_dataPath.empty() ? "Prefab" : m_dataPath.c_str())) 
	{
		Application::Instance().GetWindow().OpenFileDialog(m_dataPath, "Prefabファイルを保存", "Prefabファイル\0*.prefab*\0");
	}
	
	Utility::ImGuiHelper::DragDropTarget(MyDragDrop::GAMEOBJECT_PATH_ID, m_dataPath);

	if (ImGui::Button("Create")) 
	{
		Create();
	}
}

nlohmann::json CreateObjectComponent::Serialize()
{
	return nlohmann::json();
}

void CreateObjectComponent::LoadJson(nlohmann::json _json)
{
	m_dataPath = _json["dataPath"].get<std::string>();
}
