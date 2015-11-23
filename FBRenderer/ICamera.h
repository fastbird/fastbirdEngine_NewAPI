#pragma once
namespace fastbird{
	class BoundingVolume;
	class Mat44;
	class ICamera{
	public:
		enum MatrixType {
			View,
			InverseView,
			InverseViewProj,
			Proj, // Projection
			InverseProj,
			ViewProj,
			NumMatrices,
		};
		virtual const Mat44& GetMatrix(MatrixType type) = 0;
		virtual const Transformation& GetTransformation() const = 0;
		virtual bool IsCulled(BoundingVolume* pBV) const = 0;
		virtual const Vec3& GetPosition() const = 0;
		/*
		virtual Vec2I WorldToScreen(const Vec3& pos) = 0;
		
		virtual void GetNearFar(Real& nearPlane, Real& farPlane) const = 0;
		virtual Real GetFOV() const = 0;
		virtual void SetWidth(Real width) = 0;
		virtual void SetHeight(Real height) = 0;
		virtual Real GetWidth() const = 0;
		virtual Real GetHeight() const = 0;
		virtual void SetCurrent(bool cur) = 0;*/
	};
}