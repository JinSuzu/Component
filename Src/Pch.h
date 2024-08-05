#pragma once
//===============================================
//
// プリコンパイル済みヘッダー
//  ここに書いたものは初回のみ解析されるため、コンパイル時間が高速になる。
//  全てのcppからインクルードされる必要がある。
//
//===============================================


//===============================================
//
// 基本
//
//===============================================
#pragma comment(lib,"winmm.lib")

#define NOMINMAX
#include <windows.h>
#include <stdio.h>

#include <wrl/client.h>

//===============================================
//
// STL
//
//===============================================
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <array>
#include <vector>
#include <stack>
#include <list>
#include <iterator>
#include <queue>
#include <algorithm>
#include <memory>
#include <random>
#include <fstream>
#include <iostream>
#include <sstream>
#include <functional>
#include <thread>
#include <atomic>
#include <mutex>
#include <future>
#include <filesystem>
#include <typeinfo>
#include <typeindex>


//===============================================
//
// Direct3D11
//
//===============================================
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"dxgi.lib")

#include <d3dcommon.h>
#include <dxgi.h>
#include <d3d11.h>

#include <DirectXMath.h>
#include <DirectXCollision.h>

// DirectX Tool Kit
#pragma comment(lib, "DirectXTK.lib")
#pragma comment(lib, "DirectXTKAudioWin8.lib")
#include <SimpleMath.h>
#include <Audio.h>

// DirectX Tex
#pragma comment(lib, "DirectXTex.lib")
#include <DirectXTex.h>

//===============================================
//
// Effekseer
//
//===============================================
#ifdef _DEBUG
#pragma comment(lib, "Effekseerd.lib")
#pragma comment(lib, "EffekseerRendererDX11d.lib")
#else
#pragma comment(lib, "Effekseer.lib")
#pragma comment(lib, "EffekseerRendererDX11.lib")
#endif

#include <Effekseer.h>
#include <EffekseerRendererDX11.h>

//===============================================
//
// imgui
//
//===============================================
//#define IMGUI_DEFINE_PLACEMENT_NEW
#define USE_IMGUI_API
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_stdlib.h"
#include "imgui/ImGuizmo-master/ImGuizmo.h"

/////json/////
#include "nlohmann/json.hpp"
////magic_enum///
#include "magic_enum/magic_enum.hpp"

//===============================================
// 文字コード変換
//===============================================
#include <strconv.h>

//===============================================
//
// 自作Framework
//
//===============================================
#include "Framework/KdFramework.h"

//===============================================
//
// 自作関数・システム
//
//===============================================

Math::Vector3 TrigToVec2D(float _angle);

inline const char* JpText(const char8_t* _text) { return (const char*)_text; }

template <typename T>
concept ListOrVector = std::is_same_v<T, std::vector<typename T::value_type, typename T::allocator_type>> ||
                       std::is_same_v<T, std::list<typename T::value_type, typename T::allocator_type>>;
template<class T,ListOrVector container> inline container::iterator ListFind(T _target, container& list)
{
	typename container::iterator it = list.begin();
	while (it != list.end())
	{
		if (*it == _target)return it;
		it++;
	}
	return it;
}

#include "../Src/Application/Score/Score.h"
#include "System/SystemPch.h"
/*
template<class T>
static nlohmann::json SearchJson(nlohmann::json& _json,std::string _key,T _data)
{
	nlohmann::json json;
	if (_json.is_null())return json;

	for (auto& it : _json)
	for (auto& Key : it)
	{
		if (Key == _key)
		if (it[_key] == _data)
		{
			json = it;
			break;
		}
	}
	return json;
}
*/