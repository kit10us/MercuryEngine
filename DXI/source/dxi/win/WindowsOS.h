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

			std::string GetName() const override;

			const std::vector< std::string > & GetCommandLine() const override;

			core::IRenderer * GetRenderer() const override;	   

			void SetResolution( const unify::Size< unsigned int > & resolution ) override;

			void SetFullscreen( bool fullscreen ) override;

			unify::Size< unsigned int > GetResolution() const override;
			
			const Viewport & GetDefaultViewport() const override;
			
			bool GetFullscreen() const override;

			void SetHasFocus( bool hasFocus ) override;

			bool GetHasFocus() const override;

			HINSTANCE GetHInstance();

			HWND GetHWnd();

			void Startup() override;
			
			void Shutdown() override;
			
			void DebugWrite( const std::string & text ) override;

			void DebugWriteLine( const std::string & line ) override;

			void CreateWindow( HWND & hwnd );
			void CreateDirectX();
			void DestroyDirectX();

		private:
			std::string m_name;
			HINSTANCE m_hInstance;
			int m_nCmdShow;
			std::vector< std::string > m_commandLine;
			Viewport m_defaultViewport;
			bool m_fullscreen;
			bool m_hasFocus;
			HWND m_hWnd;
			std::list< HWND > m_additionalHandles; // Handles to be serviced.
			std::shared_ptr< DXRenderer > m_renderer;
		};
	}
}