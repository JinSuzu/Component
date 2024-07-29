#pragma once
class ConfigManger
	:public nlohmann::json
{
public:
	ConfigManger() { std::filesystem::create_directories(m_configPath.parent_path()); }
	bool Load(std::string_view _key);
	void Save(std::string_view _key);

private:
	std::weak_ptr<nlohmann::json> m_config;
	nlohmann::json* m_data = this;

	const std::filesystem::path m_configPath = "Asset/Data/config.ini";
public:
};