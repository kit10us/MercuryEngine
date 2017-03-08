// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

// Controls...
#include <ui/Static.h>
#include <ui/Button.h>
#include <ui/Combobox.h>
#include <ui/Listbox.h>
#include <ui/Richtext.h>
#include <ui/Treeview.h>
#include <ui/Edit.h>			
#include <ui/IWindow.h>
#include <unify/String.h>
#include <memory>
#include <map>

#include <ui/Builder.h>

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
			
		IControl* GetControl( int controlId ) const override;
		IControl* GetControl( std::string name ) const override;

		template< typename T >
		T GetControl( int controlId ) const;

		template< typename T >
		T GetControl( std::string name ) const;

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

	template< typename T >
	T Window::GetControl( int controlId ) const
	{
		IControl * control = GetControl( controlId );
		return dynamic_cast< T >( control );
	}

	template< typename T >
	T Window::GetControl( std::string name ) const
	{					
		IControl * control = GetControl( name );
		return dynamic_cast< T >( control );
	}
}