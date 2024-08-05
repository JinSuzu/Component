#pragma once
#include "../Base/EditorWindowBase.h"

class Inspector
	:public EditorWindowBase
{
public:
	void UpdateContents() override;

private:
	std::string m_keyword;
};	
