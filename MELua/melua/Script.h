// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <melua/ScriptEngine.h>
#include <me/script/IScript.h>
#include <lua.hpp>

namespace melua
{
	class Script : public me::script::IScript
	{
	public:
		Script( lua_State * state, std::string luaName, unify::Path path );
		~Script();

		void GetGlobal( std::string  );
		
		std::string SetUpValue( int funcindex, int n );

		void NewTable();

		void PushString( std::string s );

		void SetField( int index, std::string k );

		int GetField( int index, std::string );

		std::string ToString( int index );

		int SetMetatable( int index );

		int PCall( int nargs, int nresults, int errfunc);

		void Pop( int n );

		lua_State * GetState();

		unify::Path GetPath() const;

	public: // IResource...
		size_t Owners() const;
		bool Reload() override;

		lua_State * m_state;
		std::string m_luaName;
		unify::Path m_path;
	};
}
