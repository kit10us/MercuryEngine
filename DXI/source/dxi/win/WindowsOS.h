// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/core/IOS.h>
#include <dxi/core/IGame.h>
#include <dxi/core/IRenderer.h>
#include <dxi/core/Display.h>
#include <list>
#include <vector>

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

			LRESULT WndProc( HWND handle, UINT message, WPARAM wParam, LPARAM lParam );

		private:
			class Pimpl;
			std::shared_ptr< Pimpl > m_pimpl;
		};
	}
}