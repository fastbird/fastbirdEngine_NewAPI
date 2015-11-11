#pragma once
#include "FBCommonHeaders/Types.h"
namespace fastbird{
	
	DECLARE_SMART_PTR(Animation);
	DECLARE_SMART_PTR(InputLayout);
	DECLARE_SMART_PTR(IndexBuffer);
	DECLARE_SMART_PTR(VertexBuffer);
	DECLARE_SMART_PTR(Material);
	DECLARE_SMART_PTR(Scene);
	DECLARE_SMART_PTR(IRenderable);
	class IRenderable
	{
	public:
		virtual ~IRenderable() {}
		
		virtual IRenderable* Clone() const{ return 0; }

		virtual void SetName(const char* name) {}
		virtual const char* GetName() const { return ""; }
		
		virtual void OnAttachedToScene(ScenePtr pScene) = 0;
		virtual void OnDetachedFromScene(ScenePtr pScene) = 0;
		virtual bool IsAttached(ScenePtr pScene) const = 0;
		virtual bool IsAttachedAny() const = 0;

		virtual void SetMaterial(const char* name, int pass) = 0;
		virtual void SetMaterial(MaterialPtr pMat, int pass) = 0;
		virtual MaterialPtr GetMaterial(int pass = 0) const = 0;

		virtual void SetVertexBuffer(VertexBufferPtr pVertexBuffer) = 0;
		virtual void SetIndexBuffer(IndexBufferPtr pIndexBuffer) = 0;
		// override the input layout defined in material
		virtual void SetInputLayout(InputLayoutPtr i) = 0;
		
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
		virtual void SetObjFlag(unsigned flag) = 0;
		virtual unsigned GetObjFlag() const = 0;
		virtual void ModifyObjFlag(unsigned flag, bool enable) = 0;
		virtual bool HasObjFlag(unsigned flag) = 0;		
		virtual void SetShow(bool show) = 0;
		virtual bool GetShow() const = 0;

		//-------------------------------------------------------------------
		// Spatial Information
		//-------------------------------------------------------------------
		virtual bool IsSpatial() const { return false; }
		virtual void SetRadius(Real r){}
		virtual void SetDistToCam(Real dist){}
		virtual Real GetDistToCam() const { return -1; }
		virtual const Vec3& GetPos() const { return Vec3::ZERO; }
		virtual void SetPos(const Vec3& pos) {}

		//-------------------------------------------------------------------
		// Rendering
		//-------------------------------------------------------------------
		virtual void PreRender() = 0;
		virtual void Render() = 0;
		virtual void PostRender() = 0;

		virtual void SetEnableHighlight(bool highlight) {}
		const AnimationPtr GetAnimation() const { return 0; }

		virtual void SetGameType(int type) = 0;
		virtual int GetGameType() const = 0;
		virtual void SetGameId(unsigned id) = 0;
		virtual unsigned GetGameId() const = 0;
		virtual void SetGamePtr(void* ptr) = 0;
		virtual void* GetGamePtr() const = 0;
	};
}