// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/IGame.h>
#include <meedr/ui/Window.h>

namespace meedr
{
	class SceneViewer : public meedr::ui::Window
	{
	public:
		SceneViewer::SceneViewer( HWND parent, int nCmdShow, int x, int y, me::IGame * game );

		ui::IResult* OnDestroy( ui::Params params ) override;

	private:
		me::IGame * m_game;
	};
}