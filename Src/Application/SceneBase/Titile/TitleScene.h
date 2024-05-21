#pragma once
#include "../SceneBase.h"
class C_TitleScene :
    public SceneBase
{
public:
    C_TitleScene() 
	{
		id = "Title";
	}
    ~C_TitleScene() {}

    void Draw()override;
	void PreUpdate()override;
    void Update()override;
	void PostUpdate()override;
    void Init()override;
	void LoadContent()override;
private:
};

