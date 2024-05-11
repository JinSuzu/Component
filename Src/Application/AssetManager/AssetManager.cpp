#include "AssetManager.h"

std::shared_ptr<KdTexture> AssetManager::GetKdTexture(std::string _assetPath)
{
	if (m_texList[_assetPath].lock().get() == nullptr)
	{
		KdTexture* tex = new KdTexture;
		static std::string path = "Asset/Textures/";
		bool flg = tex->Load(path + _assetPath);
		
		assert(flg && "Pathミス！！");

		auto ptr = std::shared_ptr<KdTexture>(tex);

		m_texList[_assetPath] = ptr;

		return ptr;
	}


	assert(m_texList[_assetPath].use_count() && "Asset Load Missing!!!");
	return m_texList[_assetPath].lock();

}
