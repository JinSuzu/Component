﻿#pragma once

namespace Utility 
{
	namespace JsonHelper 
	{
#include "Helper/JsonHelper.h"
	}
		
	namespace ImGuiHelper
	{
#include "Helper/ImGuiHelper.h"
	}
	
	namespace ImGuizmoHelper
	{
#include "Helper/ImGuizmoHelper.h"
	}

	namespace StringHelper 
	{
		inline void DeleteWord(std::string& _str, const std::string& _word) {
			size_t pos = 0;
			while ((pos = _str.find(_word, pos)) != std::string::npos) {
				_str.erase(pos, _word.length());
			}
		}

		inline std::string PickName(std::string str, const char separator = ' ')
		{
			std::vector<std::string> out;
			std::stringstream ss(str);
			std::string buf;
			while (std::getline(ss, buf, separator))
			{
				out.push_back(buf);
			}

			return out[1];
		}

	}

	namespace TypeIDHelper 
	{
		// 型情報を取得する関数テンプレート
		template<typename T>
		inline const std::type_info& Create() {
			static const std::type_info& info = typeid(T);
			return info;
		}
	}

	namespace MathHelper
	{
		inline float ToRadians(float _deg)
		{
			return DirectX::XMConvertToRadians(_deg);
		}

		inline float ToDegrees(float _rad)
		{
			return DirectX::XMConvertToDegrees(_rad);
		}

		inline DirectX::SimpleMath::Vector3 ToDegrees(const DirectX::SimpleMath::Quaternion& _Quaternion)
		{
			DirectX::SimpleMath::Vector3 result;
			DirectX::SimpleMath::Vector3 radians = _Quaternion.ToEuler();
			result.x = ToDegrees(radians.y);
			result.y = ToDegrees(radians.x);
			result.z = ToDegrees(radians.z);
			return result;
		}
		
		inline void ToDegrees(const DirectX::SimpleMath::Quaternion& _Quaternion,DirectX::SimpleMath::Vector3& _result)
		{
			DirectX::SimpleMath::Vector3 radians = _Quaternion.ToEuler();
			_result.x = ToDegrees(radians.y);
			_result.y = ToDegrees(radians.x);
			_result.z = ToDegrees(radians.z);
		}
	}
}	