#pragma once

#include <melua/ScriptEngine.h>

namespace melua
{
	MELUADLL_API std::string GetTypename( lua_State *L );
	MELUADLL_API std::string GetTypename( lua_State *L, int index );

	MELUADLL_API int PushNil( lua_State *L );
	MELUADLL_API int PushBoolean( lua_State *L, bool value );
	MELUADLL_API int PushNumber( lua_State *L, float value );
	MELUADLL_API int PushString( lua_State *L, std::string value );

	template< typename T >
	T** CreateUserType( lua_State *L, T from )
	{
		T ** thing = (T**)( lua_newuserdata( L, sizeof( T* ) ) );
		*thing = new T{ from };
		luaL_setmetatable( L, T::Name() );
		return thing;
	}

	template< typename T >
	int PushUserType( lua_State *L, T from )
	{
		CreateUserType( L, from );
		return 1;
	}

	template< typename T >
	T* CheckUserType( lua_State *L, int index )
	{
		return *(T**)luaL_checkudata( L, index, T::Name() );
	}
}