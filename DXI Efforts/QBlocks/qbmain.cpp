// Copyright (c) 2002 - 2011, Evil Quail LLC
// All Rights Reserved

#include <dxi/core/Win32Game.h>
#include <GameLogic.h>


#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")



using namespace dxi;
using namespace core;

class MyGame : public Win32Game
{
protected:
    GameLogic m_gameLogic;

public:
	void Startup();
	bool Update( unify::Seconds elapsed, IInput & input );
	void Render();
	void Shutdown();
} game;


void MyGame::Startup()
{
	Win32Game::Startup();
    m_gameLogic.Startup();
}

bool MyGame::Update( unify::Seconds elapsed, IInput & input )
{
    m_gameLogic.Update( elapsed, input );
	return Win32Game::Update( elapsed, input );
}

void MyGame::Render()
{
    m_gameLogic.Render();
}

void MyGame::Shutdown()
{
    m_gameLogic.Shutdown();
	Win32Game::Shutdown();
}
