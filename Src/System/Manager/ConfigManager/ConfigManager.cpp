#include "ConfigManager.h"
#include "Application/main.h"

bool ConfigManger::Load(std::string_view _key)
{
	*m_data = Utility::JsonHelper::InputJson(m_configPath.string())[_key];
	bool failed = m_data->is_null();
	if (failed)KernelEngine::AddLog((std::string(_key.data()) + "config not found!\n").c_str());
	return !failed;
}

void ConfigManger::Save(std::string_view _key)
{
	nlohmann::json outPutData = Utility::JsonHelper::InputJson(m_configPath.string());
	outPutData[ _key] = *m_data;
	Utility::JsonHelper::OutputJson(outPutData, m_configPath.string());
	KernelEngine::AddLog((std::string(_key.data()) + "saved!!\n").c_str());
}