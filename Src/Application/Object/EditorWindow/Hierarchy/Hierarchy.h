#pragma once
#include "../Base/EditorWindowBase.h"

class GameObject;
namespace MyImGui
{
	template<class T> struct DragDrop;
}

class Hierarchy
	:public EditorWindowBase
{
public:
	Hierarchy();
	void UpdateContents()override;
private:
	void ImGuiGameObject(std::weak_ptr<GameObject> _obj, bool _colledSource = false);
	std::shared_ptr<MyImGui::DragDrop<std::weak_ptr<GameObject>>> m_dragDrop;
};
