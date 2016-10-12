// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/core/IOS.h>
#include <dxi/core/IGame.h>
#include <dxi/win/DirectX.h>
#include <dxi/win/DXRenderer.h>
#include <dxi/core/Display.h>
#include <list>

// Undefine Microsoft clashing defines.
#ifdef CreateWindow
#undef CreateWindow
#endif

#ifdef GetCommandLine
#undef GetCommandLine
#endif

namespace dxi
{
	namespace win
	{
		class WindowsOS : public core::IOS
		{
			WindowsOS( core::IGame * game );
		public:
			WindowsOS( core::IGame * game, HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow, WNDPROC wndProc );
			WindowsOS( core::IGame * game, HWND handle );

			virtual ~WindowsOS();

			std::string GetName() const override;

			const std::vector< std::string > & GetCommandLine() const override;

			void AddDisplay( core::Display display );
			void CreatePendingDisplays();
			void CreateDisplay( core::Display display );
			int RendererCount() const override;
			core::IRenderer * GetRenderer( int index ) const override;

			void SetHasFocus( bool hasFocus ) override;			 
			bool GetHasFocus() const override;					 
			
			HINSTANCE GetHInstance() const;
			HWND GetHandle() const;

			void BuildRenderers() override;

			void Startup() override;
			
			void Shutdown() override;
			
			void DebugWrite( const std::string & text ) override;

			void DebugWriteLine( const std::string & line ) override;

			IDirect3DDevice9 * GetDxDevice();

			LRESULT WndProc( HWND handle, UINT message, WPARAM wParam, LPARAM lParam );

		private:
			core::IGame * m_game;
			std::string m_name;
			
			HWND m_handle;
			HINSTANCE m_hInstance;
			int m_cmdShow;
			std::vector< std::string > m_commandLine;

			WNDPROC m_wndProc;

			CComPtr< IDirect3DDevice9 > m_dxDevice;

			input::IInputSource * m_keyboard;
			input::IInputSource * m_mouse;
			
			bool m_hasFocus;
			std::list< HWND > m_childHandles; // Handles to be serviced.
			std::vector< core::Display > m_pendingDisplays;
			std::vector< std::shared_ptr< DXRenderer > > m_renderers;
		};
	}
}