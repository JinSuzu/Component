#include "AssetManager.h"

std::shared_ptr<KdTexture> AssetManager::GetKdTexture(std::string _assetPath)
{
	if (m_texList[_assetPath].expired())
	{
		std::shared_ptr<KdTexture> tex = std::make_shared<KdTexture>();
		static std::string path = "Asset/Textures/";
		bool flg = tex->Load(path + _assetPath + ".png");
		
		assert(flg && "Pathミス！！");

		m_texList[_assetPath] = tex;

		return tex;
	}


	assert(m_texList[_assetPath].lock() && "Asset Load Missing!!!");
	return m_texList[_assetPath].lock();

}

std::shared_ptr<KdModelData> AssetManager::GetModelData(std::string _assetPath)
{
	if (m_modelDataList[_assetPath].expired())
	{
		std::shared_ptr<KdModelData> modelData = std::make_shared<KdModelData>();
		static std::string path = "Asset/Data/Model/";
		bool flg = modelData->Load(path + _assetPath + "/" + _assetPath + ".gltf");

		assert(flg && "Pathミス！！");

		m_modelDataList[_assetPath] = modelData;

		return modelData;
	}


	assert(m_modelDataList[_assetPath].lock() && "Asset Load Missing!!!");
	return m_modelDataList[_assetPath].lock();
}

std::shared_ptr<KdSquarePolygon> AssetManager::GetSquarePolygon(std::string _assetPath)
{
	if (m_squarePolygonList[_assetPath].expired())
	{
		std::shared_ptr<KdSquarePolygon> squarePolygon = std::make_shared<KdSquarePolygon>();
		static std::string path = "Asset/Textures/";
		squarePolygon->SetMaterial(path + _assetPath + ".png");

		m_squarePolygonList[_assetPath] = squarePolygon;

		return squarePolygon;
	}

	assert(m_squarePolygonList[_assetPath].lock() && "Asset Load Missing!!!");
	return m_squarePolygonList[_assetPath].lock();
}

