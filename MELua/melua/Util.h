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
		return PushUserType( L, value );
	}
	
	template< typename T >
	int Push( lua_State * L, T value );

	template< typename T >
	int Push( lua_State * L, T value, int args_increment ) 
	{
		args_increment++;
		Push< T >( L, value );
		return args_increment;
	}

	template< typename T, typename... Args >
	int Push( lua_State * L, T value, Args ... args, int args_increment = 0 )
	{
		args_increment++;
		return Push< T >( L, value, Args, args_count++ );
		return 
	}


	
	int Call( lua_State * L, std::string function, int return_count );


	struct CallParams {
		int arg_count;
		int return_count;
	};

	/// <summary>
	/// Final Recursive call that requires arguments
	/// </summary>
	int CallRecursive( lua_State * L, std::string function, CallParams callParams );

	/// <summary>
	/// Recursive call that requires arguments
	/// </summary>
	template< typename T >
	int CallRecursive( lua_State * L, std::string function, CallParams callParams, T t )
	{
		// Push argument.
		Push( L, t );

		++callParams.arg_count;
		
		return CallRecursive( L, function, callParams );
	}

	/// <summary>
	/// Recursive call that requires arguments
	/// </summary>
	template< typename T, typename... Args >
	int CallRecursive( lua_State * L, std::string function, CallParams callParams, T t, Args... args )
	{
		// Push argument.
		Push( L, t );

		// Increate the number of arguments we are calling with.
		++callParams.arg_count;

		return Call( L, function, callParams, arg_count, args... );
	}

	/// <summary>
	/// First stop call to a function that uses aruments.
	/// </summary>
	template< typename T, typename... Args >
	int Call( lua_State * L, std::string function, int return_count, T t, Args... args )
	{
		// Push function name.
		lua_getglobal( L, function.c_str() );

		CallParams callParams { 0, return_count };

		return CallRecursive( L, function, callParams, t, args... );
	}


	/// <summary>
	/// Replaces the "requires" lua function, so we can keep track of all the scripts we load.
	/// </summary>
	int Include( lua_State * L );

	

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