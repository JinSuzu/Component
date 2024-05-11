#include "Pch.h"


float* Vec3ToArray(Math::Vector3 _value)
{
	float array[3] =
	{
		_value.x,
		_value.y,
		_value.z
	};
	return array;
}


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
		atof(std::string(_other["x"]).c_str()),
		atof(std::string(_other["y"]).c_str())
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
	return nlohmann::json{ {"x",_value.x}, {"y",_value.y}, {"z",_value.z} };
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