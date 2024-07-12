﻿#pragma once
class Cp_Texture;
class Cp_ModelData;
class Cp_SquarePolygon;

enum class DrawType
{
	Lit = 1 << 0,
	UnLit = 1 << 1,
	Bright = 1 << 2,
	UI = 1 << 3,
	DepthOfShadow = 1 << 4,
};

class RenderManager
{
public:
	void DrawProcess();
	void PreDraw();
	void Draw();
	void DrawSprite();
	void DrawDebug();

	void AddPreDraw(std::weak_ptr<std::function<void()>>   _add) { m_preDrawList.push_back(_add); }
	void AddDraw3D(std::weak_ptr<std::function<void(UINT)>>_add) { m_draw3DList.push_back(_add); }
	void AddDraw2D(std::weak_ptr<std::function<void()>>	   _add) { m_draw2DList.push_back(_add); }
	void AddDrawDebug(std::weak_ptr<std::function<void()>> _add) { m_drawDebugList.push_back(_add); }
	void AddCamera(int _priority, std::weak_ptr<class Cp_Camera> _camera);

	void DebugViewResize(int w, int h) { m_rtp.CreateRenderTarget(w, h); }
	void DebugViewResize(const int* size) { m_rtp.CreateRenderTarget(*size, *(++size)); }

	KdRenderTargetPack const GetDebugView() { return m_rtp; }

	std::weak_ptr<class Cp_Camera> GetCamera();



	void Init();
private:
	std::list<std::weak_ptr<std::function<void()>>>		m_preDrawList;
	std::list<std::weak_ptr<std::function<void(UINT)>>>	m_draw3DList;
	std::list<std::weak_ptr<std::function<void()>>>		m_draw2DList;
	std::list<std::weak_ptr<std::function<void()>>>		m_drawDebugList;
	std::map<int, std::list<std::weak_ptr<class Cp_Camera>>> m_cameraMap;

	KdRenderTargetChanger	m_rtc;
	KdRenderTargetPack		m_rtp;
	RenderManager() {};
public:
	static RenderManager& Instance()
	{
		static RenderManager inst;
		return inst;
	}
};
