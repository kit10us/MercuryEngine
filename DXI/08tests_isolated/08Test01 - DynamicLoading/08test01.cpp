// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxi/core/Game.h>

#include <dxi/GeometryGroup.h>
#include <dxi/Terra.h>
#include <dxi/shapes/ShapeCreators.h>
#include <unify/Size3.h>

#include <dxilua/DXILua.h>

#pragma comment( lib, "DXILua" )

using namespace dxi;
using namespace core;

class MyGame : public Game
{
public:
	std::shared_ptr< scripting::IScriptEngine > luaSE;

	bool Setup( core::IOS & os )
	{
		os.SetResolution( unify::Size< unsigned int >( 800, 600 ) );
		os.SetFullscreen( false );
		return true;
	}

	void Startup()
	{
		Game::Startup();

		AddScriptEngine( "lua2", new dxilua::ScriptEngine( *this ) );

		HMODULE mh = LoadLibraryA( "DXILuaDLL.dll" );
		assert( mh );

		typedef bool ( __cdecl *LoaderFunction )(dxi::core::Game &);

		LoaderFunction loader = (LoaderFunction)GetProcAddress( mh, "DXILoader" );
		assert( loader );

		loader( *this );

		scripting::IScriptEngine * luaDLL = GetScriptEngine( "lua" );
		scripting::IScriptEngine * luaLIB = GetScriptEngine( "lua2" );

		luaDLL->ExecuteFile( "media/scene1.lua" );
		luaLIB->ExecuteFile( "media/scene1_2.lua" );


		FreeLibrary( mh );

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
		luaSE.reset();
	}
} game;

