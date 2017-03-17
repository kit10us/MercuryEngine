// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <melua/ScriptEngine.h>
#include <melua/ExportTexture.h>
#include <me/Game.h>

using namespace melua;
using namespace me;

int PushTexture( lua_State * state, ITexture::ptr texture )
{
	TextureProxy ** newProxy = (TextureProxy**)(lua_newuserdata( state, sizeof( TextureProxy* ) ));
	*newProxy = new TextureProxy();
	(*newProxy)->texture = texture;
	luaL_setmetatable( state, "TextureProxy" );
	return 1;
}

TextureProxy* CheckTexture( lua_State* state, int index )
{
	TextureProxy* ud = *(TextureProxy**)luaL_checkudata( state, index, "Texture" );
	return ud;
}
			   
static const luaL_Reg TextureFunctions[] =
{
	{ nullptr, nullptr }
};

int Texture_Constructor( lua_State * state )
{
	int top = lua_gettop( state );
	int type = lua_type( state, 1 );

	ITexture::ptr texture;

	auto game = dynamic_cast< Game * >( ScriptEngine::GetGame() );
	auto renderer = game->GetOS()->GetRenderer( 0 );
	
	// Allow pulling existing from manager...
	if ( top == 1 )
	{
		std::string name = lua_tostring( state, 1 );
		texture = game->GetManager< ITexture >()->Find( name );
	}
	else
	{
		std::string name = lua_tostring( state, 1 );
		unify::Path source = lua_tostring( state, 2 );

		if ( top >= 3 )
		{
			bool renderable = true;
			if ( top >= 3 )
			{
				renderable = lua_toboolean( state, 3 ) ? true : false;
			}

			bool readable = true;
			if ( top >= 4 )
			{
				readable = lua_toboolean( state, 4 ) ? true : false;
			}

			TextureParameters parameters;
			parameters.lockable = readable;
			parameters.renderable = renderable;
			parameters.source = source;

			texture = game->GetManager< ITexture >()->Add( name, source, "", &parameters );
		}
		else
		{
			texture = game->GetManager< ITexture >()->Add( name, source );
		}
	}

	TextureProxy ** textureProxy = (TextureProxy**)(lua_newuserdata( state, sizeof( TextureProxy* ) ));
	*textureProxy = new TextureProxy;
	(*textureProxy)->texture = texture;
	luaL_setmetatable( state, "Texture" );
	
	return 1;
}

int Texture_Destructor( lua_State * state )
{
	TextureProxy * TextureProxy = CheckTexture( state, 1 );
	delete TextureProxy;
	return 0;
}

void RegisterTexture( lua_State * state )
{
	ScriptEngine * se = ScriptEngine::GetInstance();
	se->AddType( "Texture", TextureFunctions, sizeof( TextureFunctions ) / sizeof( luaL_Reg ), Texture_Constructor, Texture_Destructor );
}

