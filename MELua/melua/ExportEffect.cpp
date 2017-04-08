// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <melua/ScriptEngine.h>
#include <melua/ExportEffect.h>
#include <melua/ExportTexture.h>
#include <melua/ExportPixelShader.h>
#include <melua/ExportVertexShader.h>
#include <melua/Util.h>
#include <me/Game.h>

using namespace melua;
using namespace me;
using namespace render;

char* EffectProxy::Name()
{
	return "Effect";
}

int Effect_SetTexture( lua_State* state )
{
	int top = lua_gettop( state );

	EffectProxy* proxy = CheckUserType< EffectProxy >( state, 1 );
	int stage = (int)lua_tonumber( state, 2 );
	TextureProxy* texture = CheckUserType< TextureProxy >( state, 3 );

	proxy->effect->SetTexture( stage, texture->texture );

	return 0;
}

int Effect_Constructor( lua_State * state )
{
	ScriptEngine * se = ScriptEngine::GetInstance();
	auto game = se->GetGame();
	int top = lua_gettop( state );

	if ( top < 1 )
	{
		Error( state, "Effect requires at least the name of the effect as a parameter!" );
	}

	try
	{

		if( top == 1 )
		{
			std::string name = lua_tostring( state, 1 );
			auto effect = game->GetManager< Effect >()->Find( name );
			if( effect )
			{
				return Push< EffectProxy >( state, { effect } );
			}
			else
			{
				return PushNil( state );
			}
		}
		else if( top >= 2 )
		{
			std::vector< std::string > types( GetTypenames( state ) );

			// Two paramters, both strings, then it's a name, path.
			if( unify::StringIs( types[0], "String" ) && unify::StringIs( types[1], "String" ) )
			{
				std::string name = lua_tostring( state, 1 );
				unify::Path source( lua_tostring( state, 2 ) );
				auto effect = game->GetManager< Effect >()->Add( name, source );
				return Push< EffectProxy >( state, { effect } );
			}
			else // It's a ps, vs, and N textures...
			{
				IPixelShader::ptr ps;
				IVertexShader::ptr vs;
				std::vector< ITexture::ptr > textures;
				for( size_t i = 0; i < types.size(); i++ )
				{
					if( unify::StringIs( types[i], PixelShaderProxy::Name() ) )
					{
						if( ps )
						{
							Error( state, "Attempted to create an effect with two pixel shaders!" );
						}
						ps = CheckUserType< PixelShaderProxy >( state, i + 1 )->shader;
					}
					else if( unify::StringIs( types[i], VertexShaderProxy::Name() ) )
					{
						if( vs )
						{
							Error( state, "Attempted to create an effect with two vertex shaders!" );
						}
						vs = CheckUserType< VertexShaderProxy >( state, i + 1 )->shader;
					}
					else if( unify::StringIs( types[i], TextureProxy::Name() ) )
					{
						textures.push_back( CheckUserType< TextureProxy >( state, i + 1 )->texture );
					}
				}

				if( !ps || !vs )
				{
					Error( state, "Invalid PixelShader, or VertexShader!" );
				}

				if( textures.size() > 0 )
				{
					Effect::ptr effect( new Effect( vs, ps, &textures[0], &textures[0] + textures.size() ) );
					return Push< EffectProxy >( state, { effect } );
				}
				else
				{
					Effect::ptr effect( new Effect( vs, ps ) );
					return Push< EffectProxy >( state, { effect } );
				}
			}
		}
	}
	catch( std::exception ex )
	{
		Error( state, ex.what() );
	}

	return PushNil( state );
}

int Effect_Destructor( lua_State * state )
{
	EffectProxy * effectProxy = CheckUserType< EffectProxy >( state, 1 );
	delete effectProxy;
	return 0;
}

void RegisterEffect( lua_State * state )
{
	const luaL_Reg EffectFunctions[] =
	{
		{ "SetTexture", Effect_SetTexture },
		{ nullptr, nullptr }
	};


	ScriptEngine * se = ScriptEngine::GetInstance();
	se->AddType( { "Effect", EffectFunctions, sizeof( EffectFunctions ) / sizeof( luaL_Reg ), Effect_Constructor, Effect_Destructor } );
}

