// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/IOS.h>
#include <me/game/IGame.h>
#include <me/render/IRenderer.h>
#include <me/render/Display.h>
#include <list>
#include <vector>

#include <Windows.h>

// Undefine Microsoft clashing defines.
#ifdef CreateWindow
#undef CreateWindow
#endif

#ifdef GetCommandLine
#undef GetCommandLine
#endif

namespace medx9
{
	class WindowsOS : public me::IOS
	{
	public:
		WindowsOS( me::game::IGame * gameInstance, const qxml::Element * element );

		virtual ~WindowsOS();


		me::game::IGame * GetGame() override;

		void * Feed( std::string target, void * data ) override;

		std::string GetName() const override;

		const std::vector< std::string > & GetCommandLine() const override;

		void AddDisplay( me::render::Display display );
		void CreatePendingDisplays( std::string title );
		void CreateDisplay( me::render::Display display, std::string title );
		int RendererCount() const override;
		me::render::IRenderer * GetRenderer( int index ) const override;

		void SetHasFocus( bool hasFocus ) override;			 
		bool GetHasFocus() const override;					 
			
		HINSTANCE GetHInstance() const;
		HWND GetHandle() const;

		void BuildRenderers( std::string title ) override;

		void Startup() override;
			
		void Shutdown() override;
			
		void DebugWrite( std::string text ) override;

		void DebugWriteLine( std::string line ) override;

		LRESULT WndProc( HWND handle, UINT message, WPARAM wParam, LPARAM lParam );

		unify::AssetPaths & WindowsOS::GetAssetPaths() override;

		unify::Path GetProgramPath() const override;

		unify::Path GetRunPath() const override;

		bool DeletePath( unify::Path path ) override;

		void DebugOutput( std::string message ) override;

	private:
		me::game::IGame * m_game;
		std::string m_name;
		unify::Path m_programPath;
		unify::Path m_runPath;
		std::vector< std::string > m_commandLine;
		me::input::IInputDevice * m_keyboard;
		me::input::IInputDevice * m_mouse;
		bool m_hasFocus;
		std::list< HWND > m_childHandles; // Handles to be serviced.
		std::vector< me::render::Display > m_pendingDisplays;
		std::vector< me::render::IRenderer::ptr > m_renderers;

		me::OSParameters m_osParameters;
		unify::AssetPaths m_assetPaths;
	};
}