/*
 -----------------------------------------------------------------------------
 This source file is part of fastbird engine
 For the latest info, see http://www.jungwan.net/
 
 Copyright (c) 2013-2015 Jungwan Byun
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 -----------------------------------------------------------------------------
*/

#pragma once
#include "FBCommonHeaders/platform.h"
#include "FBCommonHeaders/VectorMap.h"
#include "LuaUtils.h"
#include "FBCommonHeaders/SpinLock.h"
namespace fastbird
{
	class FB_DLL_PUBLIC LuaObject;
	//-----------------------------------------------------------------------------
	class LuaTableIterator
	{
		lua_State* mL;
	public:

		LuaTableIterator(const LuaObject& table);
		~LuaTableIterator();
		typedef std::pair<LuaObject, LuaObject> KeyValue;
		bool GetNext(KeyValue& outKeyValue);
	};

	//-----------------------------------------------------------------------------
	class LuaSequenceIterator
	{
		lua_State* mL;
		size_t mLen;
		size_t mCurIdx;

	public:
		LuaSequenceIterator(const LuaObject& sequence);
		~LuaSequenceIterator();
		bool GetNext(LuaObject& out);

	};

	//-----------------------------------------------------------------------------
	class FB_DLL_PUBLIC LuaObject
	{
		int mRef;
		lua_State* mL;
		int mType;
		std::string mName;
		LuaObject* mSelf;
		static VectorMap<int, unsigned> sUsedCount;
		static SpinLock<true, false> sUsedCountGuard;

		static void AddUsedCount(int ref);
		static bool ReleaseUsedCount(int ref);

	public:
		LuaObject();
		LuaObject(lua_State* L);
		// index will not be popped.
		LuaObject(lua_State* L, int index, bool pop = false);
		LuaObject(lua_State* L, const char* globalName);
		LuaObject(const LuaObject& other);
		LuaObject& operator=(const LuaObject& other);
		~LuaObject();

		void SetSelf(const LuaObject& other);
		void FindFunction(lua_State* L, const char* functName);

		lua_State* GetLuaState() const { return mL; }

		void NewTable(lua_State* L);

		void SetGlobalName(lua_State* L, const char* globalName);
		const char* GetGlobalName() const { return mName.c_str(); }
		bool IsFunction() const;
		bool IsMethod() const; // A method is also a function.
		bool IsTable() const;
		bool IsString() const;
		bool IsNumber() const;
		bool IsBool() const;
		bool IsNil() const;
		bool IsValid(bool nilIsValid = false) const;
		unsigned GetType() const { return mType; }
		LuaObject GetField(const char* fieldName) const;
		LuaTableIterator GetTableIterator() const;
		LuaSequenceIterator GetSequenceIterator() const;

		LuaObject SetFieldTable(const char* fieldName) const;
		void SetField(const char* fieldName, double num) const;
		void SetField(const char* fieldName, int num) const;
		void SetField(const char* fieldName, unsigned num) const;
		void SetField(const char* fieldName, bool b) const;
		void SetField(const char* fieldName, const char* str) const;
		void SetField(const char* fieldName, const Vec3Tuple& v) const; /// Vec3
		void SetField(const char* fieldName, const Vec3ITuple& v) const; /// Vec3I
		void SetField(const char* fieldName, const Vec4Tuple& v) const; // Vec4
		void SetField(const char* fieldName, const Vec2Tuple& v) const; // Vec2
		void SetField(const char* fieldName, const std::tuple<int, int>& v) const; // Vec2I
		void SetField(const char* fieldName, const TransformationTuple& v) const; // Transformation
		void SetField(const char* fieldName, LuaObject& value) const;
		void SetField(LuaObject& key, LuaObject& value) const;

		LuaObject SetSeqTable(int n) const;
		LuaObject GetSeqTable(int n);
		void SetSeq(int n, const char* str)  const;
		void SetSeq(int n, char* str) const;
		void SetSeq(int n, unsigned num) const;
		void SetSeq(int n, int num) const;
		void SetSeq(int n, float num) const;
		void SetSeq(int n, double num) const;
		void SetSeq(int n, const Vec4Tuple& val) const;
		void SetSeq(int n, const Vec3ITuple& val) const;
		void SetSeq(int n, const Vec3Tuple& val) const;
		void SetSeq(int n, LuaObject& value) const;
		template<class T>
		void SetSeq(int n, T* val) const
		{
			LUA_STACK_WATCHER w(mL, "void SetSeq(int n, T* val)");
			PushToStack();
			luaW_push(mL, val);
			lua_rawseti(mL, -2, n);
			lua_pop(mL, 1);
		}
		template <class T>
		void SetSeqTemplate(int n, T v) const
		{
			LUA_STACK_WATCHER w(mL, "void SetSeqTemplate(int n, T v)");
			PushToStack();
			luaW_push(mL, v);
			lua_rawseti(mL, -2, n);
			lua_pop(mL, 1);
		}

		void AppendTable(LuaObject& table) const;

		double GetNumberAt(int index) const;
		unsigned GetUnsignedAt(int index) const;
		LuaObject GetTableAt(int index) const;
		std::string GetString(std::string& def = std::string()) const;
		float GetFloat(float def = 0.f) const;
		double GetDouble(double def = 0.) const;
		int GetInt(int def = 0) const;
		unsigned GetUnsigned(unsigned def = 0) const;
		unsigned GetUnsignedFromString(unsigned def = 0) const;
		bool GetBoolWithDef(bool def = false) const;
		Vec3Tuple GetVec3(const Vec3Tuple& def = Vec3Tuple(0., 0., 0.)) const;
		Vec4Tuple GetVec4(const Vec4Tuple& def = Vec4Tuple(0., 0., 0., 0.)) const;
		Vec3ITuple GetVec3I(const Vec3ITuple& def = Vec3ITuple(0, 0, 0)) const;
		Vec2Tuple GetVec2(const Vec2Tuple& def = Vec2Tuple(0., 0.)) const;
		Vec2ITuple GetVec2I(const Vec2ITuple& def = Vec2ITuple(0, 0)) const;
		QuatTuple GetQuat(const QuatTuple& def = QuatTuple(0., 0., 0., 0.))const;
		TransformationTuple GetTransformation(const TransformationTuple& def = TransformationTuple(
			1., 0., 0.,
			0., 1., 0.,
			0., 0., 1.,
			1., 0., 0., 0.,
			0., 0., 0.,
			1., 1., 1.,
			true, true, true
			)) const;

		std::string GetString(bool& success) const;
		float GetFloat(bool& success) const;
		int GetInt(bool& success) const;
		unsigned GetUnsigned(bool& success) const;
		unsigned GetUnsignedFromString(bool& success) const;
		bool GetBool(bool& success) const;
		Vec3Tuple GetVec3(bool& success) const;
		Vec4Tuple GetVec4(bool& success) const;
		Vec3ITuple GetVec3I(bool& success) const;
		Vec2Tuple GetVec2(bool& success) const;
		Vec2ITuple GetVec2I(bool& success) const;
		QuatTuple GetQuat(bool& success)const;
		TransformationTuple GetTransformation(bool& success) const;

		void PushToStack() const;
		bool Call();
		bool CallWithManualArgs(unsigned numArgs, unsigned numRets);

		void Clear();

		unsigned GetLen() const;

		bool operator==(const LuaObject& other) const;

		bool HasFunction() const;

	private:
		void CheckType();
	};

	LuaObject GetLuaVar(lua_State* L, const char* var, const char* file = 0);

}