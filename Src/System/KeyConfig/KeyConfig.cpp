#include "KeyConfig.h"
#include "../../SceneBase/Manager/SceneManager.h"
#include "../../SceneBase/SceneBase.h"

void KeyConfig::AddKeyBind(std::string _name, UINT KeyID)
{
	//auto key = m_keyConfigs[SceneManager::Instance().GetNowScene().lock()->GetName()].find(_name);
	//if (key != m_keyConfigs[SceneManager::Instance().GetNowScene().lock()->GetName()].end())return;
	//m_keyConfigs[SceneManager::Instance().GetNowScene().lock()->GetName()].({ _name,  KeyID });
}

KeyConfig::KeyConfig()
{
	//m_keyConfigs.resize(SceneManager::Instance().GetNowSceneNum() + 1);
	/*nlohmann::json json = InPutJson(Path);
	int num = 0;
	for (auto& SceneJson : json)
	{
		num++;
		for (auto& KeyBindJson : SceneJson)
		{
			KeyBind keyBind;
			keyBind.keyID = KeyBindJson["KeyID"];
			m_keyConfigs[num][KeyBindJson["Name"]] = keyBind;
		}
	}*/
}

KeyConfig::~KeyConfig()
{
	nlohmann::json json;



	MyJson::OutPutJson(json, Path);
}
