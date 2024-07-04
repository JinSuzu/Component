#include "Inspector.h"
#include "../../Game/GameObject.h"
#include "../../Game/Manager/GameObjectManager.h"
#include "../../../ImGuiHelper/ImGuiEditor.h"
#include "../../../main.h"

void Inspector::Update()
{
	std::weak_ptr<GameObject> object =
		Application::Instance().GetEditor().lock()->GetEditObject();

	GameObjectManager::ImGuiGameObject(object);
}
