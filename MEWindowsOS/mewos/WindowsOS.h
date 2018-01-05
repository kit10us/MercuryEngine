// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <mewos/MEWindowsOS.h>
#include <me/os/DefaultOS.h>
#include <me/game/IGame.h>
#include <me/render/IRenderer.h>
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

namespace mewos
{
	class WindowsOS : public me::IOS
	{
	public:
		WindowsOS( me::os::DefaultOS & defaultOS, const qxml::Element * element, me::render::IRendererFactory::ptr rendererFactory );

		virtual ~WindowsOS();

		me::game::IGame * GetGame() override;

		me::os::IDebug * Debug() override;
		const me::os::IDebug * Debug() const override;

		void * Feed( std::string target, void * data ) override;

		std::string GetName() const override;

		std::vector< std::string > GetCommandLine() const override;

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

		LRESULT WndProc( HWND handle, UINT message, WPARAM wParam, LPARAM lParam );

		unify::AssetPaths & GetAssetPaths() override;

		unify::Path GetProgramPath() const override;

		unify::Path GetRunPath() const override;

	private:
		me::game::IGame * m_game;
		me::os::IDebug * m_debug;
		me::render::IRendererFactory::ptr m_rendererFactory;
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
		HWND m_mainWindow;
	};
}