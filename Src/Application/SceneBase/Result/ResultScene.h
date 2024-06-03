#pragma once
#include "../SceneBase.h"
class ResultScene :
    public SceneBase
{
public:
    ResultScene()
	{
		m_name = "Result";
	}
    ~ResultScene() {}

	void PreUpdate()override;
	void Update()override;
	void PostUpdate()override;
    void Init()override;
private:
};

