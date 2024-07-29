#pragma once
#include "../Base/EditorWindowBase.h"

class GameObject;
namespace Utility::ImGuiHelper
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
	std::shared_ptr<Utility::ImGuiHelper::DragDrop<std::weak_ptr<GameObject>>> m_dragDrop;
};
