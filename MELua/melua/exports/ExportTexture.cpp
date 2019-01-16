// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <melua/ScriptEngine.h>
#include <melua/exports/ExportTexture.h>
#include <melua/Util.h>
#include <me/game/Game.h>

using namespace melua;
using namespace me;
using namespace render;

char* TextureProxy::Name()
{
	return "Texture";
}	   

int Texture_Constructor( lua_State * state )
{
	int top = lua_gettop( state );
	int type = lua_type( state, 1 );

	ITexture::ptr texture;

	ScriptEngine * se = ScriptEngine::GetInstance();
	auto gameInstance = se->GetGame();
	auto renderer = gameInstance->GetOS()->GetRenderer( 0 );
	
	try
	{
		// Allow pulling existing from manager...
		if( top == 1 )
		{
			std::string value = lua_tostring( state, 1 );
			unify::Path path( value );
			texture = gameInstance->GetManager< ITexture >()->Find( value );
			if( ! texture )
			{
				texture = gameInstance->GetManager< ITexture >()->Add( value, unify::Path{ path } );
			}
		}
		else
		{
			std::string name = lua_tostring( state, 1 );
			unify::Path source( lua_tostring( state, 2 ) );

			if( top >= 3 )
			{
				TextureParameters parameters;
				parameters.lockable = true;
				if( top >= 3 )
				{
					parameters.lockable = lua_toboolean( state, 3 ) ? true : false;
				}
				
				parameters.renderable = true;
				bool readable = true;
				if( top >= 4 )
				{
					parameters.renderable = lua_toboolean( state, 4 ) ? true : false;
				}

				parameters.format = render::Format::R32G32B32A32_Float;
				if( top >= 4 )
				{
					parameters.format = render::Format::FromString( lua_tostring( state, 5 ) );
				}


				parameters.source = source;

				texture = gameInstance->GetManager< ITexture >()->Add( name, source, unify::Path(), &parameters );
			}
			else
			{
				texture = gameInstance->GetManager< ITexture >()->Add( name, source );
			}
		}
	}
	catch( std::exception ex )
	{
		Error( state, ex.what() );
	}

	if( !texture )
	{
		Error( state, "Unable to find texture by name or filename!" );
	}

	return Push< TextureProxy >( state, { texture } );
}

int Texture_Destructor( lua_State * state )
{
	TextureProxy * proxy = CheckUserType< TextureProxy >( state, 1 );
	delete proxy;
	return 0;
}

void RegisterTexture( lua_State * state )
{
	ScriptEngine * se = ScriptEngine::GetInstance();
	const luaL_Reg functions[] =
	{
		{ nullptr, nullptr }
	};
	Type type = { TextureProxy::Name(), functions, Texture_Constructor, Texture_Destructor };
	se->AddType( type );
}

