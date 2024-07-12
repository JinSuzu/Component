#include "Hierarchy.h"
#include "../../Game/GameObject.h"
#include "../../../ImGuiHelper/ImGuiEditor.h"
#include "../../Game/Manager/GameObjectManager.h"
#include "../../../Object/EditorWindow/Prefab/Prefab.h"
#include "../../../main.h"

void Hierarchy::UpdateContents()
{
	ImGui::BeginChild("##ObjectChild");
	{
		for (auto& obj : GameObjectManager::Instance().GetObjectList())if(obj->GetParent().expired())ImGuiGameObject(obj);
	}
	ImGui::EndChild();
	Prefab::TargetGameObject(std::weak_ptr<GameObject>());
	if (ImGui::IsItemClicked(1))ImGui::OpenPopup("CreateObject");
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

void Hierarchy::ImGuiGameObject(std::weak_ptr<GameObject> _obj)
{
	ImGuiTreeNodeFlags treeFlg = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;
	std::list<std::weak_ptr<GameObject>> childs = _obj.lock()->GetChilds();
	if (childs.empty())treeFlg = ImGuiTreeNodeFlags_Leaf;

	bool flg = ImGui::TreeNodeEx((_obj.lock()->GetName() + "##" + std::to_string(_obj.lock()->GetInstanceID())).c_str(), treeFlg);
	if ((ImGui::IsItemClicked(0) || ImGui::IsItemClicked(1)))Application::Instance().GetEditor().lock()->SetEditObject(_obj);
	Prefab::TargetGameObject(_obj);
	Prefab::SourceGameObject(_obj);

	if (flg)
	{
		int i = 0;
		std::list<std::weak_ptr<GameObject>>::iterator child = childs.begin();
		while (child != childs.end())
		{
			if (child->expired())
			{
				child = childs.erase(child);
				continue;
			}

			ImGuiGameObject(*child);
			child++;
		}
		ImGui::TreePop();
	}
}
