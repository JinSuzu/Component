#include "Inspector.h"
#include "../../../Application/Object/Game/GameObject.h"
#include "../../../Application/Object/Game/Manager/GameObjectManager.h"
#include "../../../Application/main.h"

void Inspector::UpdateContents()
{
	std::weak_ptr<GameObject> object = Editor::Instance().GetEditObject();
	if(object.lock())GameObjectManager::ImGuiGameObject(object);
}
