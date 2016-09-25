// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxi/core/Game.h>
#include <dxi/GeometryGroup.h>
#include <dxi/Terra.h>
#include <dxi/shapes/ShapeCreators.h>
#include <unify/Size3.h>
#include <dxi/win/DXILib.h>

#include <dxilua/DXILua.h>

#pragma comment( lib, "DXILua" )

using namespace dxi;
using namespace core;

class MyGame : public Game
{
public:
	bool Setup( core::IOS & os )
	{
		os.SetResolution( unify::Size< unsigned int >( 800, 600 ) );
		os.SetFullscreen( false );
		return true;
	}

	void Startup()
	{
		Game::Startup();

		AddScriptEngine( "lua", std::shared_ptr< scripting::IScriptEngine >( new dxilua::ScriptEngine( this ) ) );

		scripting::IScriptEngine * lua = GetScriptEngine( "lua" );

		lua->ExecuteFile( "media/scene1.lua" );
	}

	bool Update( unify::Seconds elapsed, IInput & input )
	{
		return Game::Update( elapsed, input );
	}

	void Render()
	{
		Game::Render();
	}

	void Shutdown()
	{
		Game::Shutdown();
	}
} game;

