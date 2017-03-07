// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

// Controls...
#include <meedr/ui/Static.h>
#include <meedr/ui/Button.h>
#include <meedr/ui/Combobox.h>
#include <meedr/ui/Listbox.h>
#include <meedr/ui/Richtext.h>
#include <meedr/ui/Treeview.h>
#include <meedr/ui/Edit.h>			

#include <meedr/ui/IWindow.h>
#include <unify/String.h>
#include <memory>
#include <map>

#include <meedr/Create.h>

namespace meedr
{
	namespace ui
	{
		class Window : public IWindow
		{
			// -- Create data --
		private:
			HINSTANCE m_hInstance;
			std::wstring m_className;
			IWindow * m_parent;
			HWND m_parentHandle;
			HWND m_handle;
			create::container::Container * m_rootContainer;
			create::container::Container * m_currentParent;

		public:
			Window( HWND parent, std::wstring className );
			Window( IWindow* parent, std::wstring className );
			virtual ~Window();

			void AddContainer( create::container::Container * container );
			void AddControl( create::IControl * control, std::string name = std::string() );

			/// <summary>
			/// Step down from a container to it's n'th parent.
			/// </summary?
			void StepDown( int steps = 1 );

			HWND Create( std::wstring title, int x, int y, int nCmdShow );

			HWND GetHandle() const override;
			IWindow * GetParent() const override;
			HWND GetParentHandle() const override;

			// -- Built data --
		private:
			std::map< int, create::IControl::ptr > m_controls;
			std::map< std::string, create::IControl::ptr, unify::CaseInsensitiveLessThanTest > m_controlsByName;

		public:
			HINSTANCE GetInstance() const override;
			IControl* FindControl( int controlId ) const override;
			IControl* FindControl( std::string name ) const override;

		public: // WinApi functions...
			void GetWindowRect( RECT & rect ) const override;
			void MoveWindow( int x, int y, bool repaint ) override;
			void ShowWindow( int nCmdShow ) override;
			void SetForegroundWindow() override;
			void SetText( std::string text ) override;
			std::string GetText() const override;
			int SendUserMessage( int message, Params params ) override;

		public: // Events...
			IResult* OnCreate( Params params ) override;
			IResult* OnDestroy( Params params ) override;
			IResult* OnInitDialog( Params params ) override;
			IResult* OnAfterCreate( Params params ) override;
			IResult* OnPaint( Params params ) override;
			IResult* OnControlCommand( ControlMessage message ) override;
			IResult* OnUserMessage( UserMessageData message ) override;
		};
	}
}