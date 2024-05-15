#include "TitleScene.h"
#include "../../Object/Game/GameObject.h"
#include "../../Object/Game/Manager/GameObjectManager.h"
#include "../../Component/Component.h"

void C_TitleScene::Draw2D()
{
	SceneBase::Draw2D();
}

void C_TitleScene::PreUpdate()
{
	SceneBase::PreUpdate();
}

void C_TitleScene::Update()
{
	SceneBase::Update();
}

void C_TitleScene::PostUpdate()
{
	SceneBase::PostUpdate();
}

void C_TitleScene::Init()
{
	SceneBase::Init();
}

void C_TitleScene::LoadContent()
{
	ObjectManager::Instance().CreateObject("Title/Back");
	ObjectManager::Instance().CreateObject("Title/BackCircle");
	ObjectManager::Instance().CreateObject("Title/Title");
	ObjectManager::Instance().CreateObject("Title/Enter");
}
