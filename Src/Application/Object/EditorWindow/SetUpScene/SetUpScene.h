#pragma once
#include "../Base/EditorWindowBase.h"

class SetUpScene
	:public EditorWindowBase
{
public:
	void UpdateContents()override;
private:
	void PostUpdate()override;

	void ShowCombContents();
	void ShowTableContents();

	bool SceneTag(std::pair<const std::string, std::string>& _pair);
	bool ScenePath(std::pair<const std::string, std::string>& _pair);
	bool Remove(std::pair<const std::string, std::string>& _pair);

	std::list<std::function<bool(std::pair<const std::string, std::string>&)>>m_tableContents;
public:
	SetUpScene();
};

namespace MyDragDrop
{
	bool SourceScene(std::string _path);
	void TargetScene();
}

