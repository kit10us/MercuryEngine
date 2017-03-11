// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/IGame.h>
#include <ui/Window.h>

namespace meedr
{	  
	class ComponentViewer : public ui::Window
	{
		me::IGame* m_game;
		class SceneViewer* m_sceneViewer;

	public:
		ComponentViewer( SceneViewer* parent, int nCmdShow, int x, int y, me::IGame * game );

		void UpdateComponentTypes();
		void UpdateTypeInstances();
		void UpdateTypeSubInstances();
		void UpdateComponentList();
		void UpdateComponentValues();
		void GotoComponent( std::string type, int index );

		ui::IResult* OnCreate( ui::message::Params params ) override;
		ui::IResult* OnDestroy( ui::message::Params params ) override;
		ui::IResult* OnAfterCreate( ui::message::Params ) override;
		ui::IResult* OnControlCommand( ui::message::ControlCommand message ) override;
		ui::IResult* OnNotify( ui::message::Notify message ) override;
	};
}