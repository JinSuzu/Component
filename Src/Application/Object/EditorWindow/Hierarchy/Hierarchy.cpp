#include "Hierarchy.h"
#include "../../Game/Manager/GameObjectManager.h"
#include "../../Game/GameObject.h"
#include "../../../ImGuiHelper/ImGuiEditor.h"
#include "../../../SceneBase/Manager/SceneManager.h"
#include "../../../main.h"

void Hierarchy::Update()
{
	ImGui::BeginChild("##ObjectChild");
	{
		SceneManager::Instance().m_objectMgr->ImGuiUpdate();
	}
	ImGui::EndChild();
	if (ImGui::IsItemClicked(1))ImGui::OpenPopup("CreateObject");
	Editor::TargetGameObject(std::weak_ptr<GameObject>());
	if (ImGui::BeginPopup("CreateObject"))
	{
		std::weak_ptr<GameObject>obj = m_owner->GetEditObject();
		if ( obj.lock())
		{
			static std::string path;
			ImGui::InputText("##Path", &path);
			if (ImGui::SameLine(); ImGui::Button("Save"))
			{
				nlohmann::json json = nlohmann::json::array();
				json.push_back(obj.lock()->OutPutFamilyJson());
				MyJson::OutPutJson(json, path);
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::MenuItem("Remove"))obj.lock()->Destroy();
		}
		if (ImGui::Button("Create"))GameObjectManager::CreateObject(std::string(), obj);
		ImGui::EndPopup();
	}
}
