#pragma once
#include "Mat33.h"
#include "Vec3.h"
#include "Quat.h"
#include "Plane3.h"
#include "Ray3.h"
#include <assert.h>

namespace fastbird
{
	class Plane;
	class Mat44;
	typedef std::tuple <
		// rotation
		Real, Real, Real,
		Real, Real, Real,
		Real, Real, Real,
		// quaternion
		Real, Real, Real, Real,
		// translation
		Real, Real, Real,
		// scale
		Real, Real, Real, 
		// itentity, RSSeperated, UniformScale
		bool, bool, bool> TransformationTuple;
	DECLARE_SMART_PTR(Transformation);
	class Transformation
	{
	private:

		Mat33 mMat;
		Quat mR;
		Vec3 mT;
		Vec3 mS;
		bool mIdentity, mRSSeperated, mUniformScale;

	public:

		static const Transformation IDENTITY;

		static TransformationPtr Create();
		Transformation();
		~Transformation();
		Transformation(const Quat& q);
		explicit Transformation(const TransformationTuple& t);

		void MakeIdentity ();
		void MakeUnitScale ();
		bool IsIdentity () const{
			return mIdentity;
		}
		bool IsRSSeperated() const{
			return mRSSeperated;
		}
		bool IsUniformScale () const{
			return mRSSeperated && mUniformScale;
		}

		void SetRotation (const Mat33& r);
		void SetRotation (const Quat& r);
		void SetDir(const Vec3& dir);
		void SetDirAndRight(const Vec3& dir, const Vec3& right);
		void AddRotation (const Quat& addR);
		const Quat& GetRotation() const{
			assert(mRSSeperated);
			return mR;
		}
		void SetMatrix (const Mat33& mat);
		const Mat33& GetMatrix () const{
			assert(mRSSeperated);
			return mMat;
		}
		void SetTranslation(const Vec3& t);
		void AddTranslation(const Vec3& addT);
		const Vec3& GetTranslation() const{
			return mT;
		}
		void SetScale (const Vec3& s);
		void AddScale(const Vec3& s);
		const Vec3& GetScale () const{
			assert(mRSSeperated);
			return mS;
		}
		void SetUniformScale (Real fScale);
		Real GetUniformScale () const{
			assert(mRSSeperated && mUniformScale);
			return mS.x;
		}

		Real GetNorm () const;
		Vec3 ApplyForward (const Vec3& p) const;
		void ApplyForward (int iQuantity, const Vec3* points, Vec3* output) const;

		// X = M^{-1}*(Y-T) where Y is the input and X is the output.
		Vec3 ApplyInverse (const Vec3& p) const;
		void ApplyInverse (int iQuantity, const Vec3* points, Vec3* output) const;
		Ray3 ApplyInverse(const Ray3& r) const;

		Vec3 InvertVector (const Vec3& v) const;

		Plane3 ApplyForward (const Plane3& p) const;
		AABB ApplyForward(const AABB& aabb) const;

		void Product (const Transformation& a, const Transformation& b);
		Transformation operator* (const Transformation& t) const;
		
		void Inverse (Transformation& t) const;
		void GetHomogeneous (const Mat44& hm) const;

		Vec3 GetRight() const;
		Vec3 GetForward() const;
		Vec3 GetUp() const;	

		bool operator==(const Transformation& other) const;	
		operator TransformationTuple() const;

		// for serialization
		const Mat33& _GetMat33() { return mMat; }
		const Quat& _GetQuat() { return mR; }
		const Vec3& _GetT() { return mT; }
		const Vec3& _GetS() { return mS; }
		bool _GetIdentity() { return mIdentity; }
		bool _GetRSSeperated() { return mRSSeperated; }
		bool _GetUniformScale() { return mUniformScale; }
	};
}

//// luawapper util
//template<>
//struct luaU_Impl<fastbird::Transformation>
//{
//	static fastbird::Transformation luaU_check(lua_State* L, int index)
//	{
//		fastbird::LUA_STACK_WATCHER watcher(L, "static fastbird::Transformation luaU_check(lua_State* L, int index)");
//		luaL_checktype(L, index, LUA_TTABLE);
//		int n = 1;
//		fastbird::Transformation ret;
//		auto& mat33 = ret._GetMat33();
//		for (int r = 0; r < 3; ++r)
//			for (int c = 0; c < 3; ++c){
//				lua_rawgeti(L, index, n++);
//				mat33.m[r][c] = (Real)luaL_checknumber(L, -1);
//				lua_pop(L, 1);
//			}
//
//		auto& rot = ret._GetQuat();
//		for (int i = 0; i < 4; ++i){
//			lua_rawgeti(L, index, n++);
//			*(&rot.w + i) = (Real)luaL_checknumber(L, -1);
//			lua_pop(L, 1);
//		}
//
//		auto& t = ret._GetT();
//		for (int i = 0; i < 3; ++i){
//			lua_rawgeti(L, index, n++);
//			*(&t.x + i) = (Real)luaL_checknumber(L, -1);
//			lua_pop(L, 1);
//		}
//
//		auto& s = ret._GetS();
//		for (int i = 0; i < 3; ++i){
//			lua_rawgeti(L, index, n++);
//			*(&s.x + i) = (Real)luaL_checknumber(L, -1);
//			lua_pop(L, 1);
//		}
//
//		auto& bi = ret._GetIdentity();
//		lua_rawgeti(L, index, n++);
//		assert(lua_isboolean(L, -1));
//		bi = lua_toboolean(L, -1) != 0;
//		lua_pop(L, 1);
//
//		auto& brs = ret._GetRSSeperated();
//		lua_rawgeti(L, index, n++);		
//		assert(lua_isboolean(L, -1));
//		brs = lua_toboolean(L, -1) != 0;
//		lua_pop(L, 1);
//
//		auto& bu = ret._GetUniformScale();
//		lua_rawgeti(L, index, n++);		
//		assert(lua_isboolean(L, -1));
//		bu = lua_toboolean(L, -1) != 0;
//		lua_pop(L, 1);
//
//		return ret;
//	}
//
//	static fastbird::Transformation luaU_to(lua_State* L, int index)
//	{
//		fastbird::LUA_STACK_WATCHER watcher(L, "static fastbird::Transformation luaU_to(lua_State* L, int index)");
//		int n = 1;
//		fastbird::Transformation ret;
//		auto& mat33 = ret._GetMat33();
//		for (int r = 0; r < 3; ++r)
//			for (int c = 0; c < 3; ++c){
//				lua_rawgeti(L, index, n++);
//				mat33.m[r][c] = (Real)lua_tonumber(L, -1);
//				lua_pop(L, 1);
//			}
//
//		auto& rot = ret._GetQuat();
//		for (int i = 0; i < 4; ++i){
//			lua_rawgeti(L, index, n++);
//			*(&rot.w + i) = (Real)lua_tonumber(L, -1);
//			lua_pop(L, 1);
//		}
//
//		auto& t = ret._GetT();
//		for (int i = 0; i < 3; ++i){
//			lua_rawgeti(L, index, n++);
//			*(&t.x + i) = (Real)lua_tonumber(L, -1);
//			lua_pop(L, 1);
//		}
//
//		auto& s = ret._GetS();
//		for (int i = 0; i < 3; ++i){
//			lua_rawgeti(L, index, n++);
//			*(&s.x + i) = (Real)lua_tonumber(L, -1);
//			lua_pop(L, 1);
//		}
//
//		auto& bi = ret._GetIdentity();
//		lua_rawgeti(L, index, n++);
//		bi = lua_toboolean(L, -1) != 0;
//		lua_pop(L, 1);
//
//		auto& brs = ret._GetRSSeperated();
//		lua_rawgeti(L, index, n++);
//		brs = lua_toboolean(L, -1) != 0;
//		lua_pop(L, 1);
//
//		auto& bu = ret._GetUniformScale();
//		lua_rawgeti(L, index, n++);
//		bu = lua_toboolean(L, -1) != 0;
//		lua_pop(L, 1);
//
//		return ret;
//	}
//
//	static void luaU_push(lua_State* L, const fastbird::Transformation& val)
//	{
//		fastbird::Transformation& val2 = const_cast<fastbird::Transformation&>(val);
//		luaU_push(L, val2);
//	}
//
//	static void luaU_push(lua_State* L, fastbird::Transformation& val)
//	{
//		lua_createtable(L, 22, 0);
//
//		int n = 1;		
//		auto& mat33 = val._GetMat33();
//		for (int r = 0; r < 3; ++r)
//			for (int c = 0; c < 3; ++c){
//				lua_pushnumber(L, mat33.m[r][c]);
//				lua_rawseti(L, -2, n++);
//			}
//
//		auto& rot = val._GetQuat();
//		for (int i = 0; i < 4; ++i){
//			lua_pushnumber(L, *(&rot.w+i));
//			lua_rawseti(L, -2, n++);			
//		}
//
//		auto& t = val._GetT();
//		for (int i = 0; i < 3; ++i){
//			lua_pushnumber(L, *(&t.x + i));
//			lua_rawseti(L, -2, n++);			
//		}
//
//		auto& s = val._GetS();
//		for (int i = 0; i < 3; ++i){
//			lua_pushnumber(L, *(&s.x + i));
//			lua_rawseti(L, -2, n++);			
//		}
//
//		auto& bi = val._GetIdentity();
//		lua_pushboolean(L, bi);
//		lua_rawseti(L, -2, n++);		
//
//		auto& brs = val._GetRSSeperated();
//		lua_pushboolean(L, brs);
//		lua_rawseti(L, -2, n++);
//
//		auto& bu = val._GetUniformScale();
//		lua_pushboolean(L, bu);
//		lua_rawseti(L, -2, n++);
//	}
//};