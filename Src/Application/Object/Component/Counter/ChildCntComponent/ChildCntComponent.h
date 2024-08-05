#pragma once


class ChildCntComponent
	:public Component
{
public:
	void PreUpdateContents()override;
private:
	int m_postNum = 0;
};
