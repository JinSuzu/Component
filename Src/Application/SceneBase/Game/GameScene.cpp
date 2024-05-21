#include "GameScene.h"
#include "../../Object/Game/GameObject.h"
#include "../../Object/Game/Manager/GameObjectManager.h"
#include "../../Object/Component/Component.h"


void C_GameScene::PreDraw()
{
	GameObject()
}

void C_GameScene::PreUpdate()
{
	SceneBase::PreUpdate();
}

void C_GameScene::Update()
{
	SceneBase::Update();
}

void C_GameScene::PostUpdate()
{
	SceneBase::PostUpdate();
}

void C_GameScene::Init()
{
	SceneBase::Init();
}

void C_GameScene::LoadContent()
{
	m_camera = std::make_unique<KdCamera>();
	/*auto obj1 = GameObjectManager::Instance().CreateObject("Test");
	GameObjectManager::Instance().CreateObject("TestC")
		->SetParent(obj1);*/
}
