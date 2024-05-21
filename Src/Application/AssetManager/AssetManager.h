#pragma once

class AssetManager
{
public:
	~AssetManager() {}

	std::shared_ptr<KdTexture> GetKdTexture(std::string _assetPath);
	std::shared_ptr<KdModelData> GetModelData(std::string _assetPath);
	std::shared_ptr<KdSquarePolygon> GetSquarePolygon(std::string _assetPath);
private:
	std::map<std::string, std::weak_ptr<KdTexture>>			m_texList;
	std::map<std::string, std::weak_ptr<KdModelData>>		m_modelDataList;
	std::map<std::string, std::weak_ptr<KdSquarePolygon>>	m_squarePolygonList;
public:
	static AssetManager& Instance()
	{
		static AssetManager instance;
		return instance;
	}
};