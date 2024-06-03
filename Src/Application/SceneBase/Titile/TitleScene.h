#pragma once
#include "../SceneBase.h"
class TitleScene :
    public SceneBase
{
public:
    TitleScene() 
	{
		m_name = "Title";
	}
    ~TitleScene() {}

	void PreUpdate()override;
    void Update()override;
	void PostUpdate()override;
    void Init()override;
	void LoadContent()override;
private:
};

