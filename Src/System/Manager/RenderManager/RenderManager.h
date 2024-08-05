#pragma once
class TextureComponent;
class ModelDataComponent;
class SquarePolygonComponent;


class RenderManager
{
public:
	enum class DrawType
	{
		Lit				= 1 << 0,
		UnLit			= 1 << 1,
		Bright			= 1 << 2,
		DepthOfShadow	= 1 << 3
	};

	//2D描画用の情報体の基底
	struct Asset2DPackBase
	{
		virtual void Draw() = 0;
	};

	//3D描画用の情報体の基底
	struct Asset3DPackBase
	{
		virtual void Draw(UINT _drawType) = 0;
		UINT drawType = (UINT)DrawType::Lit;
	};

	//デバック描画用の情報体の基底
	struct DebugWireFrameInfo
	{
		virtual void AddDebugWireFrame(KdDebugWireFrame& _debug) = 0;
	};

	struct TexturePack : Asset2DPackBase
	{
		void Draw()override
		{
			KdShaderManager::Instance().m_spriteShader.SetMatrix(*mat);
			KdShaderManager::Instance().m_spriteShader.DrawTex(tex, 0, 0, rect.width, rect.height, &rect);
			KdShaderManager::Instance().m_spriteShader.SetMatrix(Math::Matrix::Identity);
		}

		Math::Matrix* mat = nullptr;
		Math::Rectangle				rect;
		std::shared_ptr<KdTexture>	tex;
	};

	struct SquarePolygonComponent : Asset3DPackBase
	{
		void Draw(UINT _type)override
		{
			if (drawType & _type)
			{
				KdShaderManager::Instance().m_StandardShader.DrawPolygon(*squarePolygon, *mat, color);
			}
		}

		Math::Matrix* mat = nullptr;
		Math::Color							color = kWhiteColor;
		std::shared_ptr<KdSquarePolygon>	squarePolygon;
	};

	struct ModelDataPack : Asset3DPackBase
	{
		void Draw(UINT _type)override
		{
			if (drawType & _type)
			{
				KdShaderManager::Instance().m_StandardShader.DrawModel(*modelData, *mat, color);
			}
		}

		std::shared_ptr<KdModelData>		modelData;
		Math::Matrix* mat = nullptr;
		Math::Color							color = kWhiteColor;
	};

	struct ModelWorkPack : Asset3DPackBase
	{
		void Draw(UINT _type)override
		{
			if (drawType & _type)
			{
				KdShaderManager::Instance().m_StandardShader.DrawModel(*modelWork, *mat, color);
			}
		}

		std::shared_ptr<KdModelWork>		modelWork;
		Math::Matrix* mat = nullptr;
		Math::Color							color = kWhiteColor;
	};

	struct DebugSphereInfo : DebugWireFrameInfo
	{
		void AddDebugWireFrame(KdDebugWireFrame& _debug)override
		{
			_debug.AddDebugSphere(mat->Translation(), radius, scale, color);
		}

		Math::Matrix* mat = nullptr;
		float radius = 0.0f;
		Math::Vector3	scale = Math::Vector3::Zero;
		Math::Color		color = kWhiteColor;
	};

	struct DebugLineInfo : DebugWireFrameInfo
	{
		void AddDebugWireFrame(KdDebugWireFrame& _debug)override
		{
			Math::Vector3 pos = mat->Translation();
			_debug.AddDebugLine(start + pos, end + pos, color);
		}

		Math::Matrix* mat = nullptr;
		Math::Vector3 start = Math::Vector3::Zero;
		Math::Vector3 end = Math::Vector3::Zero;
		Math::Color   color = kWhiteColor;
	};

	struct DebugBoxInfo : DebugWireFrameInfo
	{
		void AddDebugWireFrame(KdDebugWireFrame& _debug)override
		{
			Math::Vector3 pos = mat->Translation();
			_debug.AddDebugBox(*mat, size, offset, color);
		}

		Math::Matrix* mat = nullptr;
		Math::Vector3 size = Math::Vector3::Zero;
		Math::Vector3 offset = Math::Vector3::Zero;
		Math::Color   color = kWhiteColor;
	};

public:
	void BeginDraw();
	void PreDraw();
	void Draw();
	void PostDraw();
	void EndDraw();

	void AddDraw2D(std::weak_ptr<Asset2DPackBase> _add) { m_draw2DList.push_back(_add); }
	void AddDraw3D(std::weak_ptr<Asset3DPackBase> _add) { m_draw3DList.push_back(_add); }
	KdDebugWireFrame& WorkDebugWireFrame() { return m_debugWireFrame; }

	const KdRenderTargetPack& GetDebugView() { return m_rtp; }

	void DebugViewResize(int w, int h) { m_rtp.CreateRenderTarget(w, h); }
	void DebugViewResize(const int* size) { m_rtp.CreateRenderTarget(*size, *(++size)); }

	void Init(int w, int h);

private:
	void DrawSprite();
	void DrawDebug();

	std::list<std::weak_ptr<Asset2DPackBase>>			m_draw2DList;
	std::list<std::weak_ptr<Asset3DPackBase>>			m_draw3DList;

	KdDebugWireFrame									m_debugWireFrame;

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
