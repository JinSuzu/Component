#pragma once
#include "../Base/EditorWindowBase.h"

class GameObject;

class Hierarchy
	:public EditorWindowBase
{
public:
	void UpdateContents()override;
private:
	void ImGuiGameObject(std::weak_ptr<GameObject> _obj);
};
