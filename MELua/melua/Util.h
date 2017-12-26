#pragma once

#include <melua/ScriptEngine.h>

namespace melua
{
	///<summary>
	/// Returns the name of the type on top.
	///</summary>
	MELUADLL_API std::string GetTypename( lua_State *L );

	///<summary>
	/// Returns the name of the type at stack index.
	///</summary>
	MELUADLL_API std::string GetTypename( lua_State *L, int index );

	///<summary>
	/// Returns a vector of all the types on the stack.
	///</summary>
	MELUADLL_API std::vector< std::string > GetTypenames( lua_State *L );

	MELUADLL_API int PushNil( lua_State *L );

	MELUADLL_API void Error( lua_State *L, std::string error );

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

	template< typename T >
	int Push( lua_State * L, T value ) 
	{
		return PushUserType< T >( L, value );
	}



	///////////////////////////////// Checks...

	//template< typename T >
	//typename std::enable_if< !std::is_pointer< T >::value, T* >::type Check( lua_State *L, int index );

	template< typename T >
	T Check( lua_State *L, int index );

	template<>
	MELUADLL_API bool Check( lua_State *L, int index );

	template<>
	MELUADLL_API int Check( lua_State *L, int index );

	template<>
	MELUADLL_API float Check( lua_State *L, int index );

	template<>
	MELUADLL_API std::string Check( lua_State *L, int index );

	///////////////////////////////// Pushes....


	template<>
	MELUADLL_API int Push( lua_State * L, bool value );

	template<>
	MELUADLL_API int Push( lua_State * L, std::string value );

	template<>
	MELUADLL_API int Push( lua_State * L, float value );

	template<>
	MELUADLL_API int Push( lua_State * L, int value );

	template<>
	MELUADLL_API int Push( lua_State * L, unsigned int value );
}