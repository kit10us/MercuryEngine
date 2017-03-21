// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <MEEditor.h>
#include <meedr/SceneViewer.h>
#include <meedr/ActionsComponent.h>
#include <me/Game.h>
#include <Windows.h>

me::IGame * s_game;

ui::IWindow::ptr s_rootWindow;
ui::IWindow::ptr s_engineMain;
unify::Owner::ptr s_ownership;

void OpenEditor()
{
	if (!s_engineMain)
	{
		s_engineMain.reset(new meedr::SceneViewer(s_rootWindow, s_game));
	}
}

void DestroyEditor()
{
	s_engineMain.reset();
}

extern "C" __declspec(dllexport) bool MELoader( me::IGame * game, const qxml::Element * element )
{
	s_game = game;
	s_ownership = unify::Owner::Create( "MEEditor" );

	s_rootWindow.reset(new ui::Window((HWND)game->GetOSParameters().hWnd));

	{
		auto component = new meedr::ActionsGameComponent();
		game->AddComponent( me::IGameComponent::ptr( component ) );
	}

	me::input::InputManager * inputManager = game->GetInputManager();

	if (inputManager)
	{
		auto inputs = element->FindFirstElement("inputs");
		if (inputs)
		{
			size_t failures = inputManager->AddInputActions(s_ownership, inputs, false);
			if (failures)
			{
				return false;
			}
		}
	}

	return true;
}

