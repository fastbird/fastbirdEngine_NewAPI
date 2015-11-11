#pragma once
#ifndef _Camera_Header_included_
#define _Camera_Header_included_
#include "FBCommonHeaders/Observable.h"
#include "FBInputManager/InputConsumer.h"
#include "ICameraObserver.h"

namespace fastbird
{
	DECLARE_SMART_PTR(IMouse);
	DECLARE_SMART_PTR(IKeyboard);
	DECLARE_SMART_PTR(IRenderable);
	class FB_DLL_PUBLIC Camera : public Observable<ICameraObserver>, public InputConsumer
	{
		DECLARE_PIMPL(Camera);

	public:
		enum FRUSTUM_PLANE
		{
			FRUSTUM_PLANE_NEAR   = 0,
			FRUSTUM_PLANE_FAR    = 1,
			FRUSTUM_PLANE_LEFT   = 2,
			FRUSTUM_PLANE_RIGHT  = 3,
			FRUSTUM_PLANE_TOP    = 4,
			FRUSTUM_PLANE_BOTTOM = 5
		};

		enum ObserverEvents{
			TransformChanged,
		};

		Camera();
		~Camera();

		//-------------------------------------------------------------------------
		void SetOrthogonal(bool ortho);
		void SetPosition(const Vec3& pos);
		const Vec3& GetPosition() const;
		void SetDirection(const Vec3& dir);
		void SetDirrectionAndRight(const Vec3& dir, const Vec3& right);
		void SetRotation(const Quat& rot);
		const Quat& GetRotation() const;
		void SetTransform(const Vec3& pos, const Quat& rot);
		void SetTransform(const Transformation& t);
		const Vec3 GetDirection() const;
		enum MatrixType {
			View,
			InverseView,
			InverseViewProj,
			Proj, // Projection
			InverseProj,
			ViewProj,
			NumMatrices,
		};
		const Mat44& GetMatrix(MatrixType type);
		
		// field of view in the y direction, in radians.
		void SetFOV(Real fov);
		Real GetFOV() const;
		Real GetTanHalfFOV() const;
		// width / height
		void SetAspectRatio(Real ar);
		Real GetAspectRatio() const;
		// near/far view-plane
		void SetNearFar(Real nearPlane, Real farPlane);
		void GetNearFar(Real& nearPlane, Real& farPlane) const;
		// width and height of the view volume at the near view-plane
		void SetWidth(Real width);
		void SetHeight(Real height);
		Real GetWidth() const;
		Real GetHeight() const;
		void SetName(const char* name);
		const char* GetName() const;
		void Update();
		bool IsCulled(BoundingVolume* pBV) const;
		Ray3 ScreenPosToRay(long x, long y);
		Vec2I WorldToScreen(const Vec3& worldPos) const;
		void SetYZSwap(bool enable);
		void SetTarget(IRenderablePtr pObj);
		void SetDistanceFromTarget(Real dist);
		IRenderablePtr GetTarget() const;
		void SetCurrent(bool cur);
		void OnInput(IMousePtr mouse, IKeyboardPtr keyboard);
		void SetCameraIndex(size_t idx);
		void SetEnalbeInput(bool enable);
		void SetInitialDistToTarget(Real dist);

		//-------------------------------------------------------------------
		// InputConsumer
		//-------------------------------------------------------------------
		virtual void ConsumeInput(InputManager* inputManager);

	protected:
		void UpdateFrustum();
		void ProcessInputData();
	};
}

#endif //_Camera_Header_included_