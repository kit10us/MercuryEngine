// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <melua/ScriptEngine.h>
#include <melua/exports/ExportPixelShader.h>
#include <melua/exports/ExportEffect.h>
#include <me/game/Game.h>

using namespace melua;
using namespace me;
using namespace render;

char* PixelShaderProxy::Name()
{
	return "PixelShader";
}

int PixelShader_Constructor( lua_State * state )
{
	ScriptEngine * se = ScriptEngine::GetInstance();
	auto gameInstance = se->GetGame();
	int top = lua_gettop( state );

	if( top < 1 )
	{
		Error( state, "Invalid parameters!" );
	}

	try
	{

		auto type = GetTypename( state, 1 );

		// Extract from an effect...
		if( top == 1 && unify::string::StringIs( type, EffectProxy::Name() ) )
		{
			Effect::ptr effect = CheckUserType< EffectProxy >( state, 1 )->effect;
			return Push< PixelShaderProxy >( state, { effect->GetPixelShader() } );
		}

		if( top == 1 && unify::string::StringIs( type, "String" ) )
		{
			std::string value = Check< std::string >( state, 1 );
			unify::Path asPath( value );
			if( asPath.IsExtension( "xml" ) )
			{
				auto ps = gameInstance->GetManager< IPixelShader >()->Add( value, asPath );
				return Push< PixelShaderProxy >( state, { ps } );
			}
			else
			{
				auto ps = gameInstance->GetManager< IPixelShader >()->Find( value );
				if( !ps )
				{
					luaL_error( state, "PixelShader not found!" );
				}
				else
				{
					return Push< PixelShaderProxy >( state, { ps } );
				}
			}
		}

		if( top == 2 && unify::string::StringIs( type, "String" ) && unify::string::StringIs( GetTypename( state, 2 ), "String" ) )
		{
			std::string name = Check< std::string >( state, 1 );
			unify::Path path( Check< std::string >( state, 2 ) );
			auto ps = gameInstance->GetManager< IPixelShader >()->Add( name, path );
			return Push< PixelShaderProxy >( state, { ps } );
		}
	}
	catch( std::exception ex )
	{
		Error( state, ex.what() );
	}

	return PushNil( state );
}

int PixelShader_Destructor( lua_State * state )
{
	PixelShaderProxy * proxy = CheckUserType< PixelShaderProxy >( state, 1 );
	delete proxy;
	return 0;
}

void RegisterPixelShader( lua_State * state )
{
	const luaL_Reg functions[] =
	{
		{ nullptr, nullptr }
	};


	ScriptEngine * se = ScriptEngine::GetInstance();
	Type type = { PixelShaderProxy::Name(), functions, PixelShader_Constructor, PixelShader_Destructor };
	se->AddType( type );
}

