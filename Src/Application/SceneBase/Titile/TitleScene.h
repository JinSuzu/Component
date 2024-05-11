#pragma once
#include "../SceneBase.h"
class C_TitleScene :
    public SceneBase
{
public:
    C_TitleScene() 
    {}
    ~C_TitleScene() {}

    void Draw2D()override;
	void PreUpdate()override;
    void Update()override;
	void PostUpdate()override;
    void Init()override;
	void LoadContent()override;
private:
};

