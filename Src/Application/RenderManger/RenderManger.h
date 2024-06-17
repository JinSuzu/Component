#pragma once
class Cp_Texture;
class Cp_ModelData;
class Cp_SquarePolygon;

enum DrawType
{
	Lit = 1 << 0,
	UnLit = 1 << 1,
	Bright = 1 << 2,
	UI = 1 << 3,
	DepthOfShadow = 1 << 4,
};

class RenderTarget
{
public:
	RenderTarget() {}
	~RenderTarget()
	{
		assert(bCallBegin && "Error: レンダーターゲットが生成されてません");
		if (bCallEnd)assert(bCallEnd && "Error: レンダーターゲットが終了していません");
	}

	void BeginRenderTarget();
	void EndRenderTarget();

	std::shared_ptr<KdTexture> GetResult() { return tex; }
private:
	std::shared_ptr<KdTexture> tex;
	bool bCallBegin = false;
	bool bCallEnd = false;
};

class RenderManager
{
public:
	void PreDraw();
	void Draw();
	void DrawSprite();
	void DrawDebug();

	void AddPreDraw(std::weak_ptr<std::function<void()>>   _add) { m_preDrawList.push_back(_add); }
	void AddDraw3D(std::weak_ptr<std::function<void(UINT)>>_add) { m_draw3DList.push_back(_add); }
	void AddDraw2D(std::weak_ptr<std::function<void()>>	   _add) { m_draw2DList.push_back(_add); }
	void AddDrawDebug(std::weak_ptr<std::function<void()>> _add) { m_drawDebugList.push_back(_add); }
	void AddCamera(int _priority, std::weak_ptr<class Cp_Camera> _camera);

	std::shared_ptr<KdTexture> CreateBackBuffer();

private:
	std::list<std::weak_ptr<std::function<void()>>>		m_preDrawList;
	std::list<std::weak_ptr<std::function<void(UINT)>>>	m_draw3DList;
	std::list<std::weak_ptr<std::function<void()>>>		m_draw2DList;
	std::list<std::weak_ptr<std::function<void()>>>		m_drawDebugList;
	std::map<int, std::list<std::weak_ptr<class Cp_Camera>>> m_cameraMap;


	RenderManager() {};
public:
	static RenderManager& Instance()
	{
		static RenderManager inst;
		return inst;
	}
};
