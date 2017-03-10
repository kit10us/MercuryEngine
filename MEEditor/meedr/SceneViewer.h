// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/IGame.h>
#include <me/scene/SceneManager.h>
#include <ui/Window.h>
#include <thread>

namespace meedr
{
	class SceneViewer : public ui::Window
	{
	public:
		SceneViewer::SceneViewer( IWindow* parent, int nCmdShow, int x, int y, me::IGame * game );
		~SceneViewer();

		void UpdateSceneList();
		void UpdateObjectList();

		void UpdateObject_All();
		void UpdateObject_Position( bool force = false );
		void UpdateObject_Components( bool force = false);
		void UpdateObject_ComponentValues();
		void UpdateObject_ComponentValueSelected();
		
		void OpenObjectComponent();
		
		ui::IResult* OnAfterCreate( ui::Params ) override;
		ui::IResult* OnDestroy( ui::Params params ) override;
		ui::IResult* OnControlCommand( ui::ControlMessage message ) override;
		ui::IResult* OnTimer( ui::TimerMessage message ) override;
		ui::IResult* OnNotify( ui::NotifyMessage message ) override;

	private:
		me::IGame * m_game;
		me::scene::SceneManager * m_sceneManager;
		UINT_PTR m_timer;
		me::UpdateLock::ptr m_editingLock;
	};
}