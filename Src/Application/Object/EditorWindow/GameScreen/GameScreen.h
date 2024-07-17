#pragma once
#include "../Base/EditorWindowBase.h"

class GameScreen
	:public EditorWindowBase
{
public:
	void UpdateContents()override;
	void PostUpdate()override;
private:
	std::shared_ptr<class GameObject>	m_buildCamera;
	std::weak_ptr<class Cp_BuildCamera> m_cameraController;
};
