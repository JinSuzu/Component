#include "AssetManager.h"
#include "../main.h"

std::shared_ptr<KdTexture> AssetManager::GetKdTexture(std::string _assetPath)
{
	if (m_texList[_assetPath] == nullptr)
	{
		std::shared_ptr<KdTexture> tex = std::make_shared<KdTexture>();

		if (tex->Load(_assetPath))m_texList[_assetPath] = tex;
		else Application::Instance().m_log.AddLog("Texture Load Missing!!\n");

		return tex;
	}

	return m_texList[_assetPath];

}
std::shared_ptr<KdModelData> AssetManager::GetModelData(std::string _assetPath)
{
	if (m_modelDataList[_assetPath] == nullptr)
	{
		std::shared_ptr<KdModelData> modelData = std::make_shared<KdModelData>();

		if (modelData->Load(_assetPath))m_modelDataList[_assetPath] = modelData;
		else Application::Instance().m_log.AddLog("Model Load Missing!!\n");

		return modelData;
	}

	return m_modelDataList[_assetPath];
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
		else Application::Instance().m_log.AddLog("SquarePolygon Load Missing!!\n");

		return squarePolygon;
	}

	return m_squarePolygonList[_assetPath];
}

bool MyImGui::SelectTexture(std::shared_ptr<KdTexture>& _tex, std::string& _path)
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
	ret |= TargetPictureAssetPath(_path);

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
bool MyImGui::SelectSquarePolygon(std::shared_ptr<KdSquarePolygon>& _poly, std::string& _path)
{
	bool ret = false;
	ImGui::PushID(_poly.get());

	if (_path.empty())_path = "No Texture File";
	if (ImGui::Button(_path.c_str()))Application::Instance().GetWindow().OpenFileDialog(_path, "画像ファイルを開く", "画像ファイル\0*.png\0");

	ret |= TargetPictureAssetPath(_path);

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
bool MyImGui::SelectModelData(std::shared_ptr<KdModelData>& _modelData, std::string& _path)
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
	ret |= TargetModelAssetPath(_path);

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

bool MyImGui::SourcePictureAssetPath(std::string _path)
{
	bool flg = false;
	if (flg = ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
	{
		char path[256];
		std::strncpy(path, _path.c_str(), sizeof(path));
		ImGui::SetDragDropPayload("PictureAssetPath", &path, sizeof(path), ImGuiCond_Once);
		ImGui::Text("PullPictureAssetPath");
		ImGui::EndDragDropSource();
	}
	return flg;
}
bool MyImGui::TargetPictureAssetPath(std::string& _path)
{
	bool flg = false;
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("PictureAssetPath"))
		{
			IM_ASSERT(payload->DataSize == sizeof(char[256]));
			std::string path = (char*)payload->Data;
			_path = path;

			flg = true;
		}
		ImGui::EndDragDropTarget();
	}

	return flg;
}

bool MyImGui::SourceModelAssetPath(std::string _path)
{
	bool flg = false;
	if (flg = ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
	{
		char path[256];
		std::strncpy(path, _path.c_str(), sizeof(path));
		ImGui::SetDragDropPayload("ModelAssetPath", &path, sizeof(path), ImGuiCond_Once);
		ImGui::Text("PullModelAssetPath");
		ImGui::EndDragDropSource();
	}
	return flg;
}
bool MyImGui::TargetModelAssetPath(std::string& _path)
{
	bool flg = false;
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ModelAssetPath"))
		{
			IM_ASSERT(payload->DataSize == sizeof(char[256]));
			std::string path = (char*)payload->Data;
			_path = path;

			flg = true;
		}
		ImGui::EndDragDropTarget();
	}

	return flg;
}
