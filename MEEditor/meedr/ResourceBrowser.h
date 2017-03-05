// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/IGame.h>
#include <meedr/ui/Window.h>

namespace meedr
{	  
	class ResourceBrowser : public ui::Window
	{
		me::IGame* m_game;

	public:
		ResourceBrowser( IWindow* parent, int nCmdShow, int x, int y, me::IGame * game );

		void UpdateResourceTypes();
		void UpdateResourceList();

		ui::IResult * OnCreate( ui::Params params ) override;
		ui::IResult * OnDestroy( ui::Params params ) override;
		ui::IResult * OnAfterCreate( ui::Params ) override;
		ui::IResult * OnControlCommand( ui::ControlMessage message ) override;
	};
}