// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

// Controls...
#include <wab/ui/Static.h>
#include <wab/ui/Button.h>
#include <wab/ui/Combobox.h>
#include <wab/ui/Listbox.h>
#include <wab/ui/Richtext.h>
#include <wab/ui/Treeview.h>
#include <wab/ui/Edit.h>			
#include <wab/ui/ListView.h>
#include <wab/ui/Menu.h>
#include <wab/ui/IWindow.h>
#include <unify/String.h>
#include <memory>
#include <map>

#include <wab/ui/Builder.h>

namespace ui
{
	class Window : public IWindow
	{
		// -- Create data --
	private:
		HINSTANCE m_hInstance;
		std::wstring m_className;
		IWindow* m_parent;
		HWND m_parentHandle;
		HWND m_handle;
		create::container::Container * m_rootContainer;
		create::container::Container * m_currentParent;
		std::map< int, create::IControl::ptr > m_controls;
		std::map< std::string, create::IControl::ptr, unify::CaseInsensitiveLessThanTest > m_controlsByName;
		create::Menu::ptr m_menu;

	public:
		Window( HWND handle );
		Window( IWindow* parent, std::wstring className );
		virtual ~Window();

		void AddContainer( create::container::Container * container );
		void AddControl( create::IControl * control, std::string name = std::string() );
		void AddMenu( create::Menu * menu );

		/// <summary>
		/// Step down from a container to it's n'th parent.
		/// </summary?
		void StepDown( int steps = 1 );

		HWND Create( std::wstring title, int x, int y, int nCmdShow );

		HWND GetHandle() const override;
		IWindow * GetParent() const override;
		HWND GetParentHandle() const override;

	public:
		HINSTANCE GetInstance() const override;
			
		IControl* GetControl( int controlId ) const override;
		IControl* GetControl( std::string name ) const override;

		template< typename T >
		T GetControl( int controlId ) const;

		template< typename T >
		T GetControl( std::string name ) const;

		Menu* GetMenu( HMENU handle ) override;
		MenuItem* GetMenuItem( int id ) override;
		MenuItem* GetMenuItem( std::string name ) override;

	public: // WinApi functions...
		void Close() override;
		void Destroy() override;
		void GetWindowRect( RECT & rect ) const override;
		void MoveWindow( int x, int y, bool repaint ) override;
		void ShowWindow( int nCmdShow ) override;
		void SetForegroundWindow() override;
		void SetText( std::string text ) override;
		std::string GetText() const override;
		int SendUserMessage( int message, message::Params params ) override;
		UINT_PTR SetTimer( UINT_PTR id, unsigned int elapsedInMS ) override;
		bool KillTimer( UINT_PTR id ) override;

	public: // Events...
		IResult* OnCreate( message::Params params ) override;
		IResult* OnClose( message::Params params ) override;
		IResult* OnDestroy( message::Params params ) override;
		IResult* OnInitDialog( message::Params params ) override;
		IResult* OnAfterCreate( message::Params params ) override;
		IResult* OnPaint( message::Params params ) override;
		IResult* OnControlCommand( message::ControlCommand message ) override;
		IResult* OnUser( message::User message ) override;
		IResult* OnNotify( message::Notify message ) override;
		IResult* OnTimer( message::Timer message ) override;
		IResult* OnMenuSelect( message::MenuSelect message ) override;
		IResult* OnMenuCommand( message::MenuCommand message ) override;

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