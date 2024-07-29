#include "Inspector.h"
#include "../../../Application/Object/Game/GameObject.h"
#include "../../../Application/Object/Game/Manager/GameObjectManager.h"
#include "../../../Application/main.h"

#include "../../Manager/EditorWindowManager/EditorWindowManager.h"

void Inspector::UpdateContents()
{
	std::weak_ptr<GameObject> object = Application::Instance().GetEditor().lock()->GetEditObject();
	if(object.lock())GameObjectManager::ImGuiGameObject(object);
}
