#pragma once
class ConfigManger
	:public nlohmann::json
{
public:
	ConfigManger()
		:m_configPath("Asset/Data/.config")
	{
		std::filesystem::create_directories(m_configPath.parent_path());
	}

	bool Load(std::string_view _key);
	void Save(std::string_view _key);

private:
	nlohmann::json* m_data = this;

	std::filesystem::path m_configPath;
};