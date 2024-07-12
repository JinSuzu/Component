#pragma once
class KeyConfig
{
public:
	void AddKeyBind(std::string, UINT KeyID);
private:
	struct KeyBind
	{
		bool Push = false;
		UINT keyID = 0;
	};

	std::map<std::string,std::map<std::string, KeyBind>> m_keyConfigs;
	const std::string Path = "KeyConfig/KeyConfig";

	KeyConfig();
	~KeyConfig();
public:
	static KeyConfig Instance() 
	{
		static KeyConfig inst;
		return inst;
	}
};

