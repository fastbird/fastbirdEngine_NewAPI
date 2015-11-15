#pragma once
#include "FBCommonHeaders/platform.h"
#include "FBRenderer/IRenderable.h"
namespace fastbird{
	DECLARE_SMART_PTR(MeshObject);
	class FB_DLL_PUBLIC MeshObject : public IRenderable{
		DECLARE_PIMPL_NON_COPYABLE(MeshObject);
		MeshObject();

	public:
		virtual ~MeshObject();

		//---------------------------------------------------------------------------
		// IRenderable Interfaces
		//---------------------------------------------------------------------------
		virtual IRenderablePtr Clone() const;
		virtual void SetName(const char* name);
		virtual const char* GetName() const;
		virtual int GetRenderableType() const ;
		virtual void OnAttachedToScene(ScenePtr pScene) ;
		virtual void OnDetachedFromScene(ScenePtr pScene) ;
		virtual bool IsAttached(ScenePtr pScene) const ;
		virtual bool IsAttachedAny() const ;

		virtual void SetMaterial(const char* name, int pass);
		virtual void SetMaterial(MaterialPtr pMat, int pass);
		virtual MaterialPtr GetMaterial(int pass = 0) const;
		virtual void SetVertexBuffer(VertexBufferPtr pVertexBuffer);
		virtual void SetIndexBuffer(IndexBufferPtr pIndexBuffer);
		// override the input layout defined in material
		virtual void SetInputLayout(InputLayoutPtr i);

		//-------------------------------------------------------------------
		// RenderableFlags	
		enum RenderableFlag
		{
			OF_HIDE = 0x1,
			OF_QUERYABLE = 0x2,
			OF_IGNORE_ME = 0x4, // in scene
			OF_NO_DEPTH_PASS = 0x8,
			OF_HIGHLIGHT_DEDI = 0x10,
			/**
			You can extend this enum for your own purpose like
			OF_MY_FLAG = OF_LAST << 1,
			*/
			OF_LAST = 0x10,
		};
		virtual void SetObjFlag(unsigned flag);
		virtual unsigned GetObjFlag() const;
		virtual void ModifyObjFlag(unsigned flag, bool enable);
		virtual bool HasObjFlag(unsigned flag);
		virtual void SetShow(bool show);
		virtual bool GetShow() const;

		//-------------------------------------------------------------------
		// Spatial Information
		virtual bool IsSpatial() const;
		virtual void SetRadius(Real r);
		virtual void SetDistToCam(Real dist);
		virtual Real GetDistToCam() const;
		virtual const Vec3& GetPosition() const;
		virtual void SetPosition(const Vec3& pos);
		virtual BoundingVolume* GetBoundingVolume();

		//-------------------------------------------------------------------
		// Rendering
		virtual void PreRender(const RenderParam& renderParam, RenderParamOut* renderParamOut);
		virtual void Render(const RenderParam& renderParam, RenderParamOut* renderParamOut);
		virtual void PostRender(const RenderParam& renderParam, RenderParamOut* renderParamOut);

		virtual void SetEnableHighlight(bool highlight);
		const AnimationPtr GetAnimation() const;

		//-------------------------------------------------------------------
		// Debugging features
		virtual void SetGameType(int type);
		virtual int GetGameType() const;
		virtual void SetGameId(unsigned id);
		virtual unsigned GetGameId() const;
		virtual void SetGamePtr(void* ptr);
		virtual void* GetGamePtr() const;
	};
}