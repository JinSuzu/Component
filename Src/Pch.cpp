#include "Pch.h"


Math::Vector3 TrigToVec2D(float _angle)
{
	_angle = DirectX::XMConvertToRadians(_angle);
	return Math::Vector3(cos(_angle), sin(_angle), 0);
}

//===============================================
//
// 自作関数・システム
//
//===============================================
Math::Vector3 JsonToVec3(nlohmann::json _other)
{
	return Math::Vector3
	(
		_other["x"],
		_other["y"],
		_other["z"]
	);
}
Math::Vector2 JsonToVec2(nlohmann::json _other)
{
	return Math::Vector2
	(
		(float)atof(std::string(_other["x"]).c_str()),
		(float)atof(std::string(_other["y"]).c_str())
	);
}
Math::Rectangle JsonToRect(nlohmann::json _other)
{
	return Math::Rectangle
	(
		_other["x"],
		_other["y"],
		_other["width"],
		_other["height"]
	);
}
nlohmann::json Vec3ToJson(Math::Vector3 _value)
{
	return nlohmann::json
	{
		{"x",_value.x},
		{"y",_value.y},
		{"z",_value.z}
	};
}
nlohmann::json RectToJson(Math::Rectangle _value)
{
	return nlohmann::json
	{
		{"x",_value.x},
		{"y",_value.y},
		{"width",_value.width},
		{"height",_value.height}
	};
}

void OutPutJson(nlohmann::json _json, std::string _path)
{
	std::ofstream file(JsonDataPath(_path));
	if (file.is_open()) {
		file << _json.dump(4);  // 読みやすい形式でファイルに書き出す
		file.close();
	};
}

nlohmann::json InPutJson(std::string _path)
{
	std::ifstream file(JsonDataPath(_path));
	if (!file.is_open())return nlohmann::json();
	nlohmann::json json;
	file >> json;
	return json;
}

bool ImGuiWindowCenterButton(std::string label)
{
	// ボタンの幅と高さを取得
	ImVec2 button_size = ImGui::CalcTextSize(label.c_str());
	button_size.x += ImGui::GetStyle().FramePadding.x * 2.0f;
	button_size.y += ImGui::GetStyle().FramePadding.y * 2.0f;

	// ウィンドウの幅を取得
	float window_width = ImGui::GetWindowSize().x;

	// ボタンを中央揃えにするためのx位置を計算
	float x_pos = (window_width - button_size.x) / 2.0f;

	// 位置を設定してボタンを描画
	ImGui::SetCursorPosX(x_pos);
	return ImGui::SmallButton(label.c_str());
}

bool ImGuiTreeCenterButton(std::string label)
{
	//ボタンのサイズを取得
	ImVec2 button_size = ImGui::CalcTextSize(label.c_str());
	button_size.x += ImGui::GetStyle().FramePadding.x * 2.0f;
	button_size.y += ImGui::GetStyle().FramePadding.y * 2.0f;

	// ツリーノードの幅を取得
	float tree_node_width = ImGui::GetContentRegionAvail().x;

	// ボタンを中央揃えにするためのx位置を計算
	float x_pos = (tree_node_width - button_size.x) / 2.0f;

	// 位置を設定してボタンを描画
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + x_pos);
	return ImGui::Button(label.c_str());
}

bool ImGuiCheckBoxBit(std::string _name, unsigned int& _ID, unsigned int _checkID)
{
	bool flg = _ID & _checkID;
	bool chenge = ImGui::Checkbox(_name.c_str(), &flg);
	if (flg) _ID |= _checkID;
		else _ID &= (~_checkID);
	return chenge;
}
