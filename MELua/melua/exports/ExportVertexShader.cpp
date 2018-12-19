// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <melua/ScriptEngine.h>
#include <melua/exports/ExportVertexShader.h>
#include <melua/exports/ExportEffect.h>
#include <me/game/Game.h>

using namespace melua;
using namespace me;
using namespace render;

char* VertexShaderProxy::Name()
{
	return "VertexShader";
}

int VertexShader_Constructor( lua_State * state )
{
	ScriptEngine * se = ScriptEngine::GetInstance();
	auto gameInstance = se->GetGame();
	int top = lua_gettop( state );

	if( top < 1 )
	{
		luaL_error( state, "Invalid parameters!" );
	}

	try
	{
		auto type = GetTypename( state, 1 );

		// Extract from an effect...
		if( top == 1 && unify::string::StringIs( type, EffectProxy::Name() ) )
		{
			Effect::ptr effect = CheckUserType< EffectProxy >( state, 1 )->effect;
			return Push< VertexShaderProxy >( state, { effect->GetVertexShader() } );
		}

		if( top == 1 && unify::string::StringIs( type, "String" ) )
		{
			std::string value = Check< std::string >( state, 1 );
			unify::Path asPath( value );
			if( asPath.IsExtension( "xml" ) )
			{
				auto vs = gameInstance->GetManager< IVertexShader >()->Add( asPath );
				return Push< VertexShaderProxy >( state, { vs } );
			}
			else
			{
				auto vs = gameInstance->GetManager< IVertexShader >()->Find( value );
				if( !vs )
				{
					luaL_error( state, "VertexShader not found!" );
				}
				else
				{
					return Push< VertexShaderProxy >( state, { vs } );
				}
			}
		}

		if( top == 2 && unify::string::StringIs( type, "String" ) && unify::string::StringIs( GetTypename( state, 2 ), "String" ) )
		{
			std::string name = Check< std::string >( state, 1 );
			unify::Path path( Check< std::string >( state, 2 ) );
			auto ps = gameInstance->GetManager< IVertexShader >()->Add( name, path );
			return Push< VertexShaderProxy >( state, { ps } );
		}
	}
	catch( std::exception ex )
	{
		Error( state, ex.what() );
	}

	return PushNil( state );
}

int VertexShader_Destructor( lua_State * state )
{
	VertexShaderProxy * proxy = CheckUserType< VertexShaderProxy >( state, 1 );
	delete proxy;
	return 0;
}

void RegisterVertexShader( lua_State * state )
{
	const luaL_Reg functions[] =
	{
		{ nullptr, nullptr }
	};


	ScriptEngine * se = ScriptEngine::GetInstance();
	Type type = { VertexShaderProxy::Name(), functions, VertexShader_Constructor, VertexShader_Destructor };
	se->AddType( type );
}

