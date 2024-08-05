#include "Inspector.h"
#include "../../../Application/Object/Game/GameObject.h"
#include "../../../Application/Object/Game/Manager/GameObjectManager.h"
#include "../../../Application/main.h"

void Inspector::UpdateContents()
{
	std::weak_ptr<GameObject> object = Editor::Instance().GetEditObject();
	if (object.expired())return;

	ImGui::InputText("Name", object.lock()->WorkName());
	object.lock()->ImGuiComponents();

	if (Utility::ImGuiHelper::SmallButtonWindowCenter("AddComponent"))ImGui::OpenPopup("Components");
	if (ImGui::BeginPopup("Components"))
	{
		ImGui::SeparatorText("Component");

		for (const auto& it : ComponentFactory::Instance().GetRegistry())
		{
			if (!ImGui::MenuItem(it.second.name.data()))continue;

			object.lock()->AddComponent(it.first);
			break;

		}
		ImGui::EndPopup();
	}
}
