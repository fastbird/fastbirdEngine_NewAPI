#pragma once
#include <string>

namespace fastbird{
	class Vec2I;
	enum CVAR_CATEGORY
	{
		CVAR_CATEGORY_SERVER,
		CVAR_CATEGORY_CLIENT,
	};

	enum CVAR_TYPE
	{
		CVAR_TYPE_INT,
		CVAR_TYPE_REAL,
		CVAR_TYPE_STRING,
		CVAR_TYPE_VEC2I,
	};

	struct CVar
	{
		CVar(const char* _name, const int _def, int& _storage,
			CVAR_CATEGORY _category, const std::string& _desc);
		CVar(const char* _name, const float _def, float& _storage,
			CVAR_CATEGORY _category, const std::string& _desc);
		// up to three characters.
		CVar(const char* _name, const std::string& _def, std::string& _storage,
			CVAR_CATEGORY _category, const std::string& _desc);
		CVar(const char* _name, const Vec2I& _def, Vec2I& _storage,
			CVAR_CATEGORY _category, const std::string& _desc);

		int GetInt() const;
		float GetFloat() const;
		std::string& GetString() const;
		Vec2I& GetVec2I() const;
		void SetData(const std::string& data);
		std::string GetData() const;

		// make sure lower case.
		std::string mName;
		CVAR_CATEGORY mCategory;
		CVAR_TYPE mType;
		void* mStorage;
		std::string mDesc;
	};

	typedef std::function<void(StringVector&)> CFunc;
	//--------------------------------------------------------------------------
	struct ConsoleCommand
	{
		ConsoleCommand(const std::string& name, CFunc f,
			const std::string& desc);

		// make sure use only low case.
		std::string mName;
		CFunc mFunc;
		std::string mDesc;
	};

	class ICVarObserver
	{
	public:
		virtual bool OnChangeCVar(CVar* pCVar) = 0;
	};
}