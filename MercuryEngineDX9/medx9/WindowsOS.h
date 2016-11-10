// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/IOS.h>
#include <me/IGame.h>
#include <me/IRenderer.h>
#include <me/Display.h>
#include <list>
#include <vector>

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
		WindowsOS( me::IGame * game );

		virtual ~WindowsOS();

		void * Feed( std::string target, void * data ) override;

		std::string GetName() const override;

		const std::vector< std::string > & GetCommandLine() const override;

		void AddDisplay( me::Display display );
		void CreatePendingDisplays();
		void CreateDisplay( me::Display display );
		int RendererCount() const override;
		me::IRenderer * GetRenderer( int index ) const override;

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

		me::OSParameters m_osParameters;
	};
}