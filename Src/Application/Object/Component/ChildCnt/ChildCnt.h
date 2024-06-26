#pragma once
#include "../Component.h"

class Cp_ChildCnt
	:public Component
{
public:
	void PreUpdateContents()override;
private:
	int m_postNum = 0;
};
