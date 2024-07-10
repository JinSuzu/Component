#pragma once
#include "../Base/EditorWindowBase.h"

class GameScreen
	:public EditorWindowBase
{
public:
	void Update()override;
private:
	std::shared_ptr<class GameObject>	m_buildCamera;
};
