#include "GameScene.h"
#include "../../Object/Object.h"
#include "../../Object/Manager/ObjectManager.h"
#include "../../Component/Component.h"

void C_GameScene::Draw2D()
{
	SceneBase::Draw2D();
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
	ObjectManager::Instance().CreateObject("Game/Home");
	ObjectManager::Instance().CreateObject("Game/Field");
	ObjectManager::Instance().CreateObject("Game/Line01");
	ObjectManager::Instance().CreateObject("Game/Line02");
	ObjectManager::Instance().CreateObject("Game/PlayerCircle");
	ObjectManager::Instance().CreateObject("Game/Recycle");
	ObjectManager::Instance().CreateObject("Game/ArrowBord");
}
