#pragma once
#include "../Base/EditorWindowBase.h"

class GameScreen
	:public EditorWindowBase
{
public:
	void UpdateContents()override;

	void BeginChildOption()override;
	void EndChildOption()override;
private:
	void Screen(ImVec2* _resultSize = nullptr);
	std::shared_ptr<class GameObject>	m_buildCamera;
	std::weak_ptr<class Cp_BuildCamera> m_cameraController;

	ImGuizmo::OPERATION m_zmoPreation = ImGuizmo::OPERATION::TRANSLATE;

	ImVec2 m_imageSize;
};
