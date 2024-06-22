#pragma once
#include "../HitResult.h"

class Cp_HitResultBlock
	:public Cp_HitResult
{
public:
	void UpdateContents()override;
	void PostUpdateContents()override;
private:
	bool m_hit = false;
};
