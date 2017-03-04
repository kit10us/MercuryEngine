// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <meedr/ui/container/Container.h>
#include <meedr/ui/container/Canvas.h>
#include <meedr/ui/container/StackPanel.h>
#include <meedr/ui/Static.h>
#include <meedr/ui/Button.h>
#include <meedr/ui/Combobox.h>
#include <meedr/ui/Listbox.h>
#include <meedr/ui/Richtext.h>
#include <meedr/ui/Treeview.h>

#include <meedr/ui/IWIndow.h>
#include <unify/String.h>
#include <memory>
#include <map>

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
			HWND m_parentHandle;
			HWND m_handle;
			container::Container * m_rootContainer;
			container::Container * m_currentParent;

		public:
			Window( HWND parent, std::wstring className );
			virtual ~Window();

			void AddContainer( container::Container * container );
			void AddControl( IControl * control, std::string name = std::string() );

			/// <summary>
			/// Step down from a container to it's n'th parent.
			/// </summary?
			void StepDown( int steps = 1 );

			HWND Create( std::wstring title, int x, int y, int nCmdShow );

			HWND GetHandle() const;
			HWND GetParentHandle() const override;

			// -- Built data --
		private:
			std::map< int, std::string > m_controls;
			std::map< std::string, int > m_controlsByName;

		protected:
			int RegistryControl( std::string name ) override;

		public:
			HINSTANCE GetInstance() const override;
			std::string FindControl( int controlId ) const override;
			int GetControl( std::string name ) const override;

		public: // WinApi functions...
			void MoveWindow( int x, int y, bool repaint ) override;

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