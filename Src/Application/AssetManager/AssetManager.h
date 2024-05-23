#pragma once

class AssetManager
{
	~AssetManager() {}
public:
	template<typename T>
	void TransformAsset(T& _asset,std::string _path)
	{
		std::function<void()>Fn;
		switch (std::hash<std::type_index>()(typeid(T)))
		{
		case TexHash:
			Fn = [&]() {_asset = GetKdTexture(_path); };
			break;
		case ModelDataHash:
			Fn = [&]() { _asset = GetModelData(_path); };
			break;
		case SquarePolygonHash:
			Fn = [&]() {_asset = GetSquarePolygon(_path); };
			break;
		}

		if (Fn)m_tasks.push_back(Fn);
	}

	void Loading() 
	{
		while (true)
		{
			for (auto& task : m_tasks) 
			{
				task();
			}
		}
	}

	std::shared_ptr<KdTexture> GetKdTexture(std::string _assetPath);
	std::shared_ptr<KdModelData> GetModelData(std::string _assetPath);
	std::shared_ptr<KdSquarePolygon> GetSquarePolygon(std::string _assetPath);
private:
	std::map<std::string, std::weak_ptr<KdTexture>>			m_texList;
	std::map<std::string, std::weak_ptr<KdModelData>>		m_modelDataList;
	std::map<std::string, std::weak_ptr<KdSquarePolygon>>	m_squarePolygonList;

	std::thread m_thread;
	std::list<std::function<void()>> m_tasks;

	const size_t TexHash = std::hash<std::type_index>()(typeid(KdTexture));
	const size_t ModelDataHash = std::hash<std::type_index>()(typeid(KdModelData));
	const size_t SquarePolygonHash = std::hash<std::type_index>()(typeid(SquarePolygonHash));
public:
	static AssetManager& Instance()
	{
		static AssetManager instance;
		return instance;
	}
};