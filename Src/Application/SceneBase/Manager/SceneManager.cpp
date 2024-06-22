#include "SceneManager.h"
#include "../SceneBase.h"	
#include "../Titile/TitleScene.h"
#include "../Game/GameScene.h"
#include "../Result/ResultScene.h"
#include "../../Object/Game/GameObject.h"
#include "../../Object/Game/Manager/GameObjectManager.h"

#include "../../main.h"

#define MAKESCENE(NAME)\
class NAME##Scene :public SceneBase { public: NAME##Scene(){m_name = #NAME;} };\
m_registerScene[SceneID::##NAME] = ([=]() {return std::make_shared<NAME##Scene>(); });

void SceneManager::PreUpdate()
{
	if (m_objectMgr == nullptr)return;
	m_objectMgr->PreUpdate();
}
void SceneManager::Update()
{
	if (m_objectMgr == nullptr)return;
	m_objectMgr->Update();
}

void SceneManager::PostUpdate()
{
	if (m_objectMgr == nullptr)return;
	m_objectMgr->PostUpdate();
}

void SceneManager::Init()
{
	m_nowSceneNum = SceneID::Scene;
	m_objectMgr = std::make_shared<GameObjectManager>();
	std::string NowSceneName = magic_enum::enum_name<SceneID>(m_nowSceneNum).data();
	m_objectMgr->Load("Scene/" + NowSceneName);
}

void SceneManager::ImGuiUpdate()
{
	if (m_objectMgr == nullptr)return;
	//m_objectMgr->ImGuiUpdate();
}

void SceneManager::ShiftScene(SceneID _toSceneNum)
{
	if (m_nowSceneNum == _toSceneNum)return;
	std::string NowSceneName = magic_enum::enum_name<SceneID>(m_nowSceneNum).data();
	std::string NextSceneName = magic_enum::enum_name<SceneID>(_toSceneNum).data();

	m_nowSceneNum = _toSceneNum;
	m_objectMgr->Release("Scene/" + NowSceneName, Application::Instance().GetBuildFlg());
	m_objectMgr->Load("Scene/" + NextSceneName);
}

void SceneManager::ReLoad()
{
	std::string SceneName = magic_enum::enum_name<SceneID>(m_nowSceneNum).data();
	m_objectMgr->Release("Scene/" + SceneName, Application::Instance().GetBuildFlg());
	m_objectMgr->Load("Scene/" + SceneName);
}
