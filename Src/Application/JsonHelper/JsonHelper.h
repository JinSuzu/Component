#pragma once
namespace MyJson
{
	inline Math::Vector3 InPutVec3(nlohmann::json _json)
	{
		return Math::Vector3
		(
			_json["x"],
			_json["y"],
			_json["z"]
		);
	}
	inline Math::Vector2 InPutVec2(nlohmann::json _json)
	{
		return Math::Vector2
		(
			_json["x"],
			_json["y"]
		);
	}
	inline Math::Rectangle InputRect(nlohmann::json _json)
	{
		return Math::Rectangle
		(
			_json["x"],
			_json["y"],
			_json["width"],
			_json["height"]
		);
	}

	inline nlohmann::json OutPutVec3(Math::Vector3 _value)
	{
		return nlohmann::json
		{
			{"x",_value.x},
			{"y",_value.y},
			{"z",_value.z}
		};
	}
	inline nlohmann::json OutPutRect(Math::Rectangle _value)
	{
		return nlohmann::json
		{
			{"x",_value.x},
			{"y",_value.y},
			{"width",_value.width},
			{"height",_value.height}
		};
	}

	inline void OutPutJson(nlohmann::json _json, std::string _path)
	{
		std::ofstream file(_path);
		if (file.is_open()) {
			file << _json.dump(4);  // 読みやすい形式でファイルに書き出す
			file.close();
		};
	}
	inline nlohmann::json InPutJson(std::string _path)
	{
		std::ifstream file(_path);
		if (!file.is_open())return nlohmann::json();
		nlohmann::json json;
		file >> json;
		return json;
	}
};