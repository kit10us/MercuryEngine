// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include "MEEditor.h"
#include <meedr/SceneViewer.h>
#include <meedr/OpenEditorAction.h>
#include <me/input/InputAction.h>
#include <me/Game.h>
#include <Windows.h>

me::IGame * s_game;
ui::IWindow::ptr s_engineMain;

unify::Owner::ptr s_ownership;

void OpenEditor()
{
	if (!s_engineMain)
	{
		s_engineMain.reset(new meedr::SceneViewer(s_game));
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

	me::input::InputManager * inputManager = game->GetInputManager();

	if (inputManager)
	{
		// Attach OpenEditorActions...
		for (auto itr : element->Children("openeditor"))
		{
			me::input::IInputCondition::ptr condition = inputManager->MakeCondition(&itr);
			if (condition)
			{
				me::input::IInputAction::ptr action(
					new me::input::InputAction( 
						me::action::IAction::ptr( 
							new meedr::OpenEditorAction() 
						)
					)
				);
				condition->GetSource()->AddEvent(s_ownership, condition, action );
			}
		}
	}

	return true;
}

