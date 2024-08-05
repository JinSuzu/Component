#pragma once

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
		// 型情報を一元管理
		template<typename T>
		class TypeInfoHolder {
		public:
			static const std::type_info& getTypeInfo() {
				static const std::type_info& info = typeid(T);
				return info;
			}
		};

		// 型情報を取得する関数テンプレート（値バージョン）
		template<typename T>
		inline const std::type_info& Create(T _value) {
			return TypeInfoHolder<T>::getTypeInfo();
		}

		// 型情報を取得する関数テンプレート（型バージョン）
		template<typename T>
		inline const std::type_info& Create() {
			return TypeInfoHolder<T>::getTypeInfo();
		}
	}
}	