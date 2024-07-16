#include "ConfigManager.h"
#include "../../Application/main.h"

bool ConfigManger::Load(std::string_view _key, nlohmann::json& _date)
{
	_date = MyJson::InputJson(m_configPath.string())[_key];
	bool failed = false;
	if (failed = _date.is_null())Application::Instance().m_log.AddLog((std::string(_key.data()) + "config not found!\n").c_str());
	return !failed;
}

void ConfigManger::Save(std::string_view _key, nlohmann::json& _data)
{
	nlohmann::json json = MyJson::InputJson(m_configPath.string());
	json.emplace(_key, _data);
	MyJson::OutputJson(json, m_configPath.string());
	Application::Instance().m_log.AddLog((std::string(_key.data()) + "saved!!\n").c_str());
}
