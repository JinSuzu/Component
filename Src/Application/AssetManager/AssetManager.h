#pragma once

class AssetManager
{
public:
	~AssetManager() {}

	std::shared_ptr<KdTexture> GetKdTexture(std::string _assetPath);
private:
	std::map<std::string, std::weak_ptr<KdTexture>> m_texList;
	AssetManager() {}
public:
	static AssetManager& Instance()
	{
		static AssetManager instance;
		return instance;
	}
};