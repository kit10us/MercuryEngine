// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/core/IOS.h>
#include <dxi/win/DirectX.h>
#include <dxi/win/DXRenderer.h>
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
			WindowsOS();
		public:
			WindowsOS( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow );
			WindowsOS( HWND hWnd );

			virtual ~WindowsOS();

			virtual const std::vector< std::string > & GetCommandLine() const;

			virtual core::IRenderer * GetRenderer() const;


			virtual void SetResolution( const unify::Size< unsigned int > & resolution );
			virtual void SetFullscreen( bool fullscreen );

			virtual unify::Size< unsigned int > GetResolution() const;
			virtual const Viewport & GetDefaultViewport() const;
			virtual bool GetFullscreen() const;

			void SetHasFocus( bool hasFocus );
			bool GetHasFocus() const;

			HINSTANCE GetHInstance();
			HWND GetHWnd();
			IDirect3DDevice9 * GetDxDevice();

			virtual void Startup();
			virtual void BeforeRender();
			virtual void AfterRender();
			virtual void Shutdown();
			virtual void DebugWrite( const std::string & text );
			virtual void DebugWriteLine( const std::string & line );

			void CreateWindow( HWND & hwnd );
			void CreateDirectX();
			void DestroyDirectX();

		private:
			HINSTANCE m_hInstance;
			int m_nCmdShow;
			std::vector< std::string > m_commandLine;
			Viewport m_defaultViewport;
			bool m_fullscreen;
			bool m_hasFocus;
			HWND m_hWnd;
			std::list< HWND > m_additionalHandles; // Handles to be serviced.
			IDirect3DDevice9 * m_dxDevice;
			D3DPRESENT_PARAMETERS m_pp;
			DXRenderer * m_renderer;
		};
	}
}