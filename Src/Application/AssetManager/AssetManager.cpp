#include "AssetManager.h"
#include "../main.h"

std::shared_ptr<KdTexture> AssetManager::GetKdTexture(std::string _assetPath)
{
	if (m_texList[_assetPath] == nullptr)
	{
		std::shared_ptr<KdTexture> tex = std::make_shared<KdTexture>();
		bool flg = tex->Load(_assetPath);

		assert(flg && "Pathミス！！");

		m_texList[_assetPath] = tex;

		return tex;
	}

	assert(m_texList[_assetPath] && "Asset Load Missing!!!");
	return m_texList[_assetPath];

}

std::shared_ptr<KdModelData> AssetManager::GetModelData(std::string _assetPath)
{
	if (m_modelDataList[_assetPath] == nullptr)
	{
		std::shared_ptr<KdModelData> modelData = std::make_shared<KdModelData>();
		bool flg = modelData->Load(_assetPath);

		assert(flg && "Pathミス！！");

		m_modelDataList[_assetPath] = modelData;

		return modelData;
	}


	assert(m_modelDataList[_assetPath] && "Asset Load Missing!!!");
	return m_modelDataList[_assetPath];
}

std::shared_ptr<KdSquarePolygon> AssetManager::GetSquarePolygon(std::string _assetPath)
{
	if (m_squarePolygonList[_assetPath] == nullptr)
	{
		std::shared_ptr<KdSquarePolygon> squarePolygon = std::make_shared<KdSquarePolygon>();
		squarePolygon->SetMaterial(_assetPath);

		m_squarePolygonList[_assetPath] = squarePolygon;

		return squarePolygon;
	}

	assert(m_squarePolygonList[_assetPath] && "Asset Load Missing!!!");
	return m_squarePolygonList[_assetPath];
}

bool MyImGui::SelectTexture(std::shared_ptr<KdTexture>& _tex, std::string& _path)
{
	bool ret = false;
	ImGui::PushID(_tex.get());

	_path = "No Texture File";
	if (ImGui::Button(_path.c_str()))
	{
		if (Application::Instance().GetWindow().OpenFileDialog(_path, "画像ファイルを開く", "画像ファイル\0*.png\0"))
		{
			_tex = AssetManager::Instance().GetKdTexture(_path);
			ret = true;
		}
	}

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

	_path = "No Texture File";
	if (ImGui::Button(_path.c_str()))
	{
		if (Application::Instance().GetWindow().OpenFileDialog(_path, "画像ファイルを開く", "画像ファイル\0*.png\0"))
		{
			_poly = AssetManager::Instance().GetSquarePolygon(_path);
			ret = true;
		}
	}

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
	return ret;
}

bool MyImGui::SelectModelData(std::shared_ptr<KdModelData>& _modelData, std::string& _path)
{
	bool ret = false;
	ImGui::PushID(_modelData.get());

	_path = "No Texture File";
	if (ImGui::Button(_path.c_str()))
	{
		if (Application::Instance().GetWindow().OpenFileDialog(_path, "モデルファイルを開く", "画像ファイル\0*.png\0"))
		{
			_modelData = AssetManager::Instance().GetModelData(_path);
			ret = true;
		}
	}

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
