// Copyright (c) 2002 - 2011, Evil Quail LLC
// All Rights Reserved

#include <dxi/core/Win32Game.h>
#include <moi/MonoIntegration.h>

using namespace dxi;
using namespace core;

#include <dxi/core/Win32Game.h>
#include <moi/MonoIntegration.h>

using namespace dxi;
using namespace core;

class MyGame : public Win32Game
{
	moi::MonoIntegration * m_mono;
	moi::IGameScript * m_game;
public:

	bool Setup( setup::Configuration & config )
	{
		if ( config.GetCommandline().empty() )
		{
			MessageBoxA( 0, 
				"RunScript requires the path to a script file as a first parameter.\n"
				"Use quotes (\"example text\") to include spaces.\n"
				"syntax: RunScript.exe thescript.dll [MyGameClass [mygame]]\n", 
				"RunScript Help", MB_OK );
			return false;
		}

		m_mono = new moi::MonoIntegration();

		std::string scriptFile = config.GetCommandline().at( 0 ); 
		std::string className = ( config.GetCommandline().size() > 1 ) ? config.GetCommandline().at( 1 ) : "MyGame";
		std::string scriptName = ( config.GetCommandline().size() > 2 ) ? config.GetCommandline().at( 2 ) : "game";
		m_game = m_mono->AddGameScript( scriptName, className, scriptFile );
		m_game->Setup();

		return true; // Return false to end the program.
	}

	void Startup()
	{
		Win32Game::Startup();

		m_game->Startup();
	}

	bool Update( unify::Seconds elapsed, IInput & input )
	{
		m_game->Update( elapsed );

		// Return this.
		return Win32Game::Update( elapsed, input );
	}

	void Render()
	{
		m_game->Render();
	}

	void Shutdown()
	{
		m_game->Shutdown();

		// Do this last.
		Win32Game::Shutdown();
	}
} game;
