#pragma once
#include "../Base/EditorWindowBase.h"

class Inspector
	:public EditorWindowBase
{
public:
	void UpdateContents() override;

	static void ImGuiGameObject(std::weak_ptr<GameObject> _object);
private:
	static void ShowGameObject(std::weak_ptr<GameObject> _object);
	static void ShowComponent(std::weak_ptr<GameObject> _object);
	static void AddComponent(std::weak_ptr<GameObject> _object);
};	
