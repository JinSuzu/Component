#pragma once
class ConfigManger
{
public:
	ConfigManger() { std::filesystem::create_directories(m_configPath.parent_path()); }
	bool Load(std::string_view _key,nlohmann::json& _date);
	void Save(std::string_view _key,nlohmann::json& _data);
private:
	std::weak_ptr<nlohmann::json> m_config;

	const std::filesystem::path m_configPath = "Asset/Data/config.ini";
public:
};