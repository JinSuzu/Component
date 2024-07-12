#include "Inspector.h"
#include "../../Game/GameObject.h"
#include "../../Game/Manager/GameObjectManager.h"
#include "../../../ImGuiHelper/ImGuiEditor.h"
#include "../../../main.h"

void Inspector::UpdateContents()
{
	std::weak_ptr<GameObject> object =
		Application::Instance().GetEditor().lock()->GetEditObject();

	if(object.lock())GameObjectManager::ImGuiGameObject(object);
}
