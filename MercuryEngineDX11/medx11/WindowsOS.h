// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/IOS.h>
#include <me/IGame.h>
#include <me/IRenderer.h>
#include <me/render/Display.h>
#include <list>
#include <vector>

#define WINDOWS_LEAN_AND_MEAN
#include <Windows.h>

// Undefine Microsoft clashing defines.
#ifdef CreateWindow
#undef CreateWindow
#endif

#ifdef GetCommandLine
#undef GetCommandLine
#endif

namespace medx11
{
	class WindowsOS : public me::IOS
	{
	public:
		WindowsOS( me::IGame * game, const qxml::Element * element );

		virtual ~WindowsOS();

		me::IGame * GetGame() override;

		void * Feed( std::string target, void * data ) override;

		std::string GetName() const override;

		const std::vector< std::string > & GetCommandLine() const override;

		void AddDisplay( me::render::Display display );
		void CreatePendingDisplays( std::string title );
		void CreateDisplay( me::render::Display display, std::string title );
		int RendererCount() const override;
		me::IRenderer * GetRenderer( int index ) const override;

		void SetHasFocus( bool hasFocus ) override;			 
		bool GetHasFocus() const override;					 
			
		HINSTANCE GetHInstance() const;
		HWND GetHandle() const;

		void BuildRenderers( std::string title ) override;

		void Startup() override;
			
		void Shutdown() override;
			
		void DebugWrite( const std::string & text ) override;

		void DebugWriteLine( const std::string & line ) override;

		LRESULT WndProc( HWND handle, UINT message, WPARAM wParam, LPARAM lParam );

		unify::AssetPaths & GetAssetPaths() override;

		unify::Path GetProgramPath() const override;

		unify::Path GetRunPath() const override;

		bool DeletePath( unify::Path path ) override;

		void DebugOutput( std::string message ) override;

	private:
		me::IGame * m_game;
		std::string m_name;
		unify::Path m_programPath;
		unify::Path m_runPath;
		std::vector< std::string > m_commandLine;
		me::input::IInputDevice * m_keyboard;
		me::input::IInputDevice * m_mouse;
		bool m_hasFocus;
		std::list< HWND > m_childHandles; // Handles to be serviced.
		std::vector< me::render::Display > m_pendingDisplays;
		std::vector< me::IRenderer::ptr > m_renderers;
		me::OSParameters m_osParameters;
		unify::AssetPaths m_assetPaths;
		HWND m_mainWindow;
	};
}