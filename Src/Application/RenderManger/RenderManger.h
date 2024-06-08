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
	RenderTarget(){ BeginRenderTarget(); }
	~RenderTarget()	{
		assert(bCallEnd && "Error: レンダーターゲットが終了していません");
	}

	//result返し
	operator std::shared_ptr<KdTexture>() 
	{
		EndRenderTarget();
		return tex;
	}
private:
	void BeginRenderTarget();
	void EndRenderTarget();
	std::shared_ptr<KdTexture> tex;
	bool bCallEnd = false;
};

class RenderManager
{
public:
	void PreDraw();
	void Draw();
	void DrawSprite();

	std::shared_ptr<KdTexture> CreateBackBuffer();

	void AddPreDraw(std::weak_ptr<std::function<void()>>   _add) { m_preDrawList.push_back(_add);}
	void AddDraw3D(std::weak_ptr<std::function<void(UINT)>>_add) { m_draw3DList.push_back(_add); }
	void AddDraw2D(std::weak_ptr<std::function<void()>>	   _add) { m_draw2DList.push_back(_add); }


	
private:
	std::list<std::weak_ptr<std::function<void()>>>	m_preDrawList;
	std::list<std::weak_ptr<std::function<void(UINT)>>>	m_draw3DList;
	std::list<std::weak_ptr<std::function<void()>>>		m_draw2DList;


	RenderManager()
	{
	};
public:
	static RenderManager& Instance() 
	{
		static RenderManager inst;
		return inst;
	}
};
