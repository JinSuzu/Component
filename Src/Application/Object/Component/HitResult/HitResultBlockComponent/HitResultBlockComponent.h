#pragma once
#include "../HitResultComponent.h"

class HitResultBlockComponent
	:public Cp_HitResult
{
public:
	void UpdateContents()override;
	void PostUpdateContents()override;
private:
	bool m_hit = false;
};
