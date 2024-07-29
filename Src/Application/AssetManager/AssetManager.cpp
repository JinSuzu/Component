#include "AssetManager.h"
#include "../main.h"

std::shared_ptr<KdTexture> AssetManager::GetKdTexture(std::string _assetPath)
{
	if (m_texList[_assetPath] == nullptr)
	{
		std::shared_ptr<KdTexture> tex = std::make_shared<KdTexture>();

		if (tex->Load(_assetPath))m_texList[_assetPath] = tex;
		else Application::Instance().AddLog("Texture Load Missing!!\n");

		return tex;
	}

	return m_texList[_assetPath];

}
std::shared_ptr<KdSquarePolygon> AssetManager::GetSquarePolygon(std::string _assetPath)
{
	if (m_squarePolygonList[_assetPath] == nullptr)
	{
		std::shared_ptr<KdTexture> tex = std::make_shared<KdTexture>();
		std::shared_ptr<KdSquarePolygon> squarePolygon;

		if (tex->Load(_assetPath))
		{
			squarePolygon = std::make_shared<KdSquarePolygon>(tex);
			m_squarePolygonList[_assetPath] = squarePolygon;
		}
		else Application::Instance().AddLog("SquarePolygon Load Missing!!\n");

		return squarePolygon;
	}

	return m_squarePolygonList[_assetPath];
}
std::shared_ptr<KdModelData> AssetManager::GetModelData(std::string _assetPath)
{
	if (m_modelDataList[_assetPath] == nullptr)
	{
		std::shared_ptr<KdModelData> modelData = std::make_shared<KdModelData>();

		if (modelData->Load(_assetPath))m_modelDataList[_assetPath] = modelData;
		else Application::Instance().AddLog("Model Load Missing!!\n");

		return modelData;
	}

	return m_modelDataList[_assetPath];
}
std::shared_ptr<KdModelWork> AssetManager::GetModelWork(std::string _assetPath)
{
	if (m_modelDataList[_assetPath] == nullptr)
	{
		std::shared_ptr<KdModelData> modelData = std::make_shared<KdModelData>();

		if (modelData->Load(_assetPath)) 
		{
			std::shared_ptr<KdModelWork> temp = std::make_shared<KdModelWork>(modelData);
			m_modelWorkList[_assetPath] = temp;
			return temp;
		}
		else 
		{
			Application::Instance().AddLog("Model Load Missing!!\n");
			return nullptr;
		}
	}

	return m_modelWorkList[_assetPath];
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
			_tex = AssetManager::Instance().GetKdTexture(_path);
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
