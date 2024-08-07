#include "Inspector.h"
#include "../../../Application/Object/Game/GameObject.h"
#include "../../../Application/Object/Game/Manager/GameObjectManager.h"
#include "../../../Application/main.h"

void Inspector::UpdateContents()
{
	std::weak_ptr<GameObject> object = Editor::Instance().GetEditObject();
	ImGuiGameObject(object);
}

void Inspector::ImGuiGameObject(std::weak_ptr<GameObject> _object)
{
	if (_object.expired())return;
	ImGui::Separator();
	{
		ShowGameObject(_object);
	}
	ImGui::Separator();
	ImGui::BeginChild((ImGuiID)_object.lock().get());
	{
		ShowComponent(_object);
		AddComponent(_object);
	}
	ImGui::EndChild();
}

void Inspector::ShowGameObject(std::weak_ptr<GameObject> _object)
{
	ImGui::InputText("Name", &_object.lock()->WorkName());
	Utility::ImGuiHelper::ComboEnum("Tag", _object.lock()->WorkTag());
}
void Inspector::ShowComponent(std::weak_ptr<GameObject> _object)
{
	ImGui::SetNextItemOpen(true, ImGuiCond_Once);
	if (ImGui::TreeNode("Transform"))
	{
		_object.lock()->GetTransform().lock()->ImGuiUpdate();
		ImGui::TreePop();
	}

	int num = 0; bool deleteFlg = false;
	for (auto& it : _object.lock()->WorkComponentList())
	{
		ImGui::SetNextItemOpen(true, ImGuiCond_Once);
		std::string ImGui = std::to_string(num++) + " : " + ComponentFactory::Instance().GetRegistry().at(it->GetID()).name;
		bool flg = ImGui::TreeNode(ImGui.c_str());
		if (ImGui::SameLine(); ImGui::SmallButton(("Remove##" + ImGui).c_str()))
		{
			deleteFlg = true;
			it->Destroy();
		}

		if (flg)
		{
			ImGui::SetNextItemOpen(true, ImGuiCond_Once);
			(*it).ImGuiUpdate();
			ImGui::TreePop();
		}
	}

	if (deleteFlg == false)return;
	auto it = _object.lock()->WorkComponentList().begin();
	while (it != _object.lock()->WorkComponentList().end())
	{
		if ((*it)->GetDestroy())
		{
			it = _object.lock()->WorkComponentList().erase(it);
			continue;
		}
		it++;
	}
}
void Inspector::AddComponent(std::weak_ptr<GameObject> _object)
{
	static std::string keyword;

	ImGui::PushID((void*)_object.lock().get());
	if (Utility::ImGuiHelper::SmallButtonWindowCenter("AddComponent"))ImGui::OpenPopup("Components");
	if (ImGui::BeginPopup("Components"))
	{
		Utility::ImGuiHelper::InputText("serch", keyword);
		ImGui::SeparatorText("Component");

		for (const auto& it : ComponentFactory::Instance().GetRegistry())
		{
			if (!keyword.empty() && it.second.name.find(keyword) == std::string::npos)continue;
			if (!ImGui::MenuItem(it.second.name.data()))continue;

			_object.lock()->AddComponent(it.first);
			break;

		}
		ImGui::EndPopup();
	}
	else keyword.clear();
	ImGui::PopID();
}
