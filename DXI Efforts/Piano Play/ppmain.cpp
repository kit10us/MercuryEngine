// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxi/core/Win32Game.h>
#include <dxi/Input.h>

#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")

#include <DSound.h>

using namespace dxi;
using namespace core;

class MyGame : public Win32Game
{
protected:

public:
	void Startup();
	bool Update( unify::Seconds elapsed, IInput & input );
	void Render();
	void Shutdown();
} game;

void MyGame::Startup()
{
	Win32Game::Startup();

}

bool MyGame::Update( unify::Seconds elapsed, IInput & input )
{
	return Win32Game::Update( elapsed, input );
}

void MyGame::Render()
{
}

void MyGame::Shutdown()
{
	Win32Game::Shutdown();
}
