#pragma once

class AssetManager
{
public:

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

	std::shared_ptr<KdTexture> GetTexture(std::string _assetPath);
	bool GetTexture(std::string _assetPath, std::shared_ptr<KdTexture>& _tex);

	std::shared_ptr<KdSquarePolygon> GetSquarePolygon(std::string _assetPath);
	bool GetSquarePolygon(std::string _assetPath, std::shared_ptr<KdSquarePolygon>& _poly);

	std::shared_ptr<KdModelData> GetModelData(std::string _assetPath);
	bool GetModelData(std::string _assetPath, std::shared_ptr<KdModelData>& _model);

	std::shared_ptr<KdModelWork> GetModelWork(std::string _assetPath);
	bool GetModelWork(std::string _assetPath, std::shared_ptr<KdModelWork>& _work);

	bool SelectTexture(std::shared_ptr<KdTexture>& _tex, std::string& _path);
	bool SelectSquarePolygon(std::shared_ptr<KdSquarePolygon>& _poly, std::string& _path);
	bool SelectModelData(std::shared_ptr<KdModelData>& _modelData, std::string& _path);
	bool SelectModelWork(std::shared_ptr<KdModelWork>& _modelData, std::string& _path);

	void Init();
	void Release();

private:
	std::map<std::string, std::shared_ptr<KdTexture>>			m_texList;
	std::map<std::string, std::shared_ptr<KdModelData>>			m_modelDataList;


	std::thread m_thread;
	std::list<std::function<void()>> m_tasks;
public:

	AssetManager() {}
	~AssetManager(){}
	static AssetManager& Instance()
	{
		static AssetManager instance;
		return instance;
	}
};

namespace MyDragDrop
{
	bool SourcePicture(std::string _path);
	bool TargetPicture(std::string& _path);

	bool SourceModel(std::string _path);
	bool TargetModel(std::string& _path);
}