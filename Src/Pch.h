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
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

/////json/////
#include "../Library/tinygltf/nlohmann/json.hpp"

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

static std::string PickName(std::string str, const char separator = ' ')
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

Math::Vector3 JsonToVec3(nlohmann::json _value);
Math::Vector2 JsonToVec2(nlohmann::json _value);
Math::Rectangle JsonToRect(nlohmann::json _value);

nlohmann::json Vec3ToJson(Math::Vector3 _value);
nlohmann::json RectToJson(Math::Rectangle _value);