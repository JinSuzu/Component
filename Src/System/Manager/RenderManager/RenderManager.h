#pragma once
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
	void BeginDraw();
	void PreDraw();
	void Draw();
	void PostDraw();
	void EndDraw();

	struct Asset2DPackBase
	{
		virtual void Draw() = 0;
	};
	struct Asset3DPackBase
	{
		virtual void Draw(UINT _drawType) = 0;
		UINT drawType = (UINT)DrawType::Lit;
	};

	void AddPreDraw(std::weak_ptr<std::function<void()>>   _add)	{ m_preDrawList.push_back(_add); }
	void AddDraw2D(std::weak_ptr<Asset2DPackBase> _add)				{ m_draw2DList.push_back(_add); }
	void AddDraw3D(std::weak_ptr<Asset3DPackBase> _add)				{ m_draw3DList.push_back(_add); }
	void AddDrawDebug(std::weak_ptr<std::function<void()>> _add)	{ m_drawDebugList.push_back(_add); }

	const KdRenderTargetPack& GetDebugView() { return m_rtp; }

	void DebugViewResize(int w, int h) { m_rtp.CreateRenderTarget(w, h); }
	void DebugViewResize(const int* size) { m_rtp.CreateRenderTarget(*size, *(++size)); }

	void Init(int w, int h);

private:
	void DrawSprite();
	void DrawDebug();

	std::list<std::weak_ptr<std::function<void()>>>		m_preDrawList;
	std::list<std::weak_ptr<Asset2DPackBase>>			m_draw2DList;
	std::list<std::weak_ptr<Asset3DPackBase>>			m_draw3DList;
	std::list<std::weak_ptr<std::function<void()>>>		m_drawDebugList;

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
