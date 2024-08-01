#include "AssetManager.h"
#include "../../../Application/main.h"

std::shared_ptr<KdTexture> AssetManager::GetTexture(std::string _assetPath)
{
	std::map<std::string, std::shared_ptr<KdTexture>>::iterator it = m_texList.find(_assetPath);
	if (it == m_texList.end())
	{
		std::shared_ptr<KdTexture> tex = std::make_shared<KdTexture>();
		if (tex->Load(_assetPath))m_texList[_assetPath] = tex;
		else
		{
			KernelEngine::AddLog("Texture Load Missing!!\n");
			return nullptr;
		}

		return tex;
	}

	return it->second;

}
bool AssetManager::GetTexture(std::string _assetPath, std::shared_ptr<KdTexture>& _tex)
{
	_tex = GetTexture(_assetPath);
	return _tex != nullptr;
}

std::shared_ptr<KdSquarePolygon> AssetManager::GetSquarePolygon(std::string _assetPath)
{
	std::shared_ptr<KdTexture> tex;
	if (GetTexture(_assetPath, tex))
	{
		std::shared_ptr<KdSquarePolygon> poly = std::make_shared<KdSquarePolygon>();
		poly->SetMaterial(tex);
		return poly;
	}

	return nullptr;
}
bool AssetManager::GetSquarePolygon(std::string _assetPath, std::shared_ptr<KdSquarePolygon>& _poly)
{
	_poly = GetSquarePolygon(_assetPath);
	return _poly != nullptr;
}

std::shared_ptr<KdModelData> AssetManager::GetModelData(std::string _assetPath)
{
	std::map<std::string, std::shared_ptr<KdModelData>>::iterator it = m_modelDataList.find(_assetPath);
	if (it == m_modelDataList.end())
	{
		std::shared_ptr<KdModelData> modelData = std::make_shared<KdModelData>();
		if (modelData->Load(_assetPath))m_modelDataList[_assetPath] = modelData;
		else
		{
			KernelEngine::AddLog("Model Load Missing!!\n");
			return nullptr;
		}

		return modelData;
	}

	return  it->second;
}
bool AssetManager::GetModelData(std::string _assetPath, std::shared_ptr<KdModelData>& _model)
{
	_model = GetModelData(_assetPath);
	return _model != nullptr;
}

std::shared_ptr<KdModelWork> AssetManager::GetModelWork(std::string _assetPath)
{
	std::shared_ptr<KdModelData> data = std::make_shared<KdModelData>();
	if (GetModelData(_assetPath, data))
	{
		std::shared_ptr<KdModelWork> work = std::make_shared<KdModelWork>();
		work->SetModelData(data);
		return work;
	}

	return nullptr;
}
bool AssetManager::GetModelWork(std::string _assetPath, std::shared_ptr<KdModelWork>& _work)
{
	_work = GetModelWork(_assetPath);
	return _work != nullptr;
}

bool AssetManager::SelectTexture(std::shared_ptr<KdTexture>& _tex, std::string& _path)
{
	bool ret = false;
	ImGui::PushID(_tex.get());
	if (_path.empty())_path = "No Texture File";
	if (ImGui::Button(_path.c_str())) 
	{
		if (Application::Instance().GetWindow().OpenFileDialog(_path, "画像ファイルを開く", "画像ファイル\0*.png\0"))
		{
			_tex = AssetManager::Instance().GetTexture(_path);
			ret = true;
		}
	}
	ret |= MyDragDrop::TargetPicture(_path);

	if (ImGui::BeginPopupContextItem("SelectTexturePopupUpID"))
	{
		if (ImGui::Selectable((const char*)"解除"))
		{
			_tex = nullptr;
			ret = true;
		}
		ImGui::EndPopup();
	}
	ImGui::PopID();
	return ret;
}
bool AssetManager::SelectSquarePolygon(std::shared_ptr<KdSquarePolygon>& _poly, std::string& _path)
{
	bool ret = false;
	ImGui::PushID(_poly.get());

	if (_path.empty())_path = "No Texture File";
	if (ImGui::Button(_path.c_str()))Application::Instance().GetWindow().OpenFileDialog(_path, "画像ファイルを開く", "画像ファイル\0*.png\0");

	ret |= MyDragDrop::TargetPicture(_path);

	if (ImGui::BeginPopupContextItem("SelectSquarePolyPopupUpID"))
	{
		if (ImGui::Selectable((const char*)"解除"))
		{
			_poly = nullptr;
			ret = true;
		}
		ImGui::EndPopup();
	}
	ImGui::PopID();

	if (ret)_poly = AssetManager::Instance().GetSquarePolygon(_path);
	return ret;
}
bool AssetManager::SelectModelData(std::shared_ptr<KdModelData>& _modelData, std::string& _path)
{
	bool ret = false;
	ImGui::PushID(_modelData.get());

	if (_path.empty())_path = "No Texture File";
	if (ImGui::Button(_path.c_str())) 
	{
		if (Application::Instance().GetWindow().OpenFileDialog(_path, "モデルファイルを開く", "gltfファイル\0*.gltf\0"))
		{
			_modelData = AssetManager::Instance().GetModelData(_path);
			ret = true;
		}
	}
	ret |= MyDragDrop::TargetModel(_path);

	if (ImGui::BeginPopupContextItem("SelectModelDataPopupUpID"))
	{
		if (ImGui::Selectable((const char*)"解除"))
		{
			_modelData = nullptr;
			ret = true;
		}
		ImGui::EndPopup();
	}
	ImGui::PopID();
	return ret;
}
bool AssetManager::SelectModelWork(std::shared_ptr<KdModelWork>& _modelWork, std::string& _path)
{
	bool ret = false;
	ImGui::PushID(_modelWork.get());

	if (_path.empty())_path = "No Texture File";
	if (ImGui::Button(_path.c_str()))
	{
		if (Application::Instance().GetWindow().OpenFileDialog(_path, "モデルファイルを開く", "gltfファイル\0*.gltf\0"))
		{
			_modelWork = AssetManager::Instance().GetModelWork(_path);
			ret = true;
		}
	}
	ret |= MyDragDrop::TargetModel(_path);

	if (ImGui::BeginPopupContextItem("SelectModelDataPopupUpID"))
	{
		if (ImGui::Selectable((const char*)"解除"))
		{
			_modelWork = nullptr;
			ret = true;

		}
		ImGui::EndPopup();
	}
	ImGui::PopID();
	return ret;
}


void AssetManager::Init()
{
	ConfigManger config;
	if (config.Load("Asset"))
	{
		for (auto& it : config["Texture"])
		{
			GetTexture(it);
		}
		
	}
}
void AssetManager::Release()
{
	ConfigManger config;

	config["Texture"] = nlohmann::json::array();
	std::map<std::string, std::shared_ptr<KdTexture>>::iterator it = m_texList.begin();
	std::map<std::string, std::shared_ptr<KdTexture>>::iterator end = m_texList.end();
	while (it != end)
	{
		config["Texture"].push_back(it->first);
		it++;
	}

	config.Save("Asset");
}

namespace MyDragDrop
{
	bool SourcePicture(std::string _path)
	{
		return Utility::ImGuiHelper::DragDropSource("PicturePath", _path);
	}

	bool TargetPicture(std::string& _path)
	{
		return  Utility::ImGuiHelper::DragDropTarget("PicturePath", _path);
	}

	bool SourceModel(std::string _path)
	{
		return  Utility::ImGuiHelper::DragDropSource("ModelPath", _path);
	}
	bool TargetModel(std::string& _path)
	{
		return  Utility::ImGuiHelper::DragDropTarget("ModelPath", _path);
	}

}