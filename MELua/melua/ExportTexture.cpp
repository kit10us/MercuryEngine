// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <melua/ScriptEngine.h>
#include <melua/ExportTexture.h>
#include <melua/Util.h>
#include <me/Game.h>

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
	auto game = se->GetGame();
	auto renderer = game->GetOS()->GetRenderer( 0 );
	
	try
	{
		// Allow pulling existing from manager...
		if( top == 1 )
		{
			std::string value = lua_tostring( state, 1 );
			unify::Path path( value );
			texture = game->GetManager< ITexture >()->Find( value );
			if( ! texture )
			{
				texture = game->GetManager< ITexture >()->Add( path );
			}
		}
		else
		{
			std::string name = lua_tostring( state, 1 );
			unify::Path source( lua_tostring( state, 2 ) );

			if( top >= 3 )
			{
				bool renderable = true;
				if( top >= 3 )
				{
					renderable = lua_toboolean( state, 3 ) ? true : false;
				}

				bool readable = true;
				if( top >= 4 )
				{
					readable = lua_toboolean( state, 4 ) ? true : false;
				}

				TextureParameters parameters;
				parameters.lockable = readable;
				parameters.renderable = renderable;
				parameters.source = source;

				texture = game->GetManager< ITexture >()->Add( name, source, unify::Path(), &parameters );
			}
			else
			{
				texture = game->GetManager< ITexture >()->Add( name, source );
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
	se->AddType( { TextureProxy::Name(), functions, sizeof( functions ) / sizeof( luaL_Reg ), Texture_Constructor, Texture_Destructor } );
}

