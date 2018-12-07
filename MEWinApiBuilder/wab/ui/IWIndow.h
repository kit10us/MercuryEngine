// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <wab/ui/POD.h>
#include <wab/ui/message/Messages.h>
#include <memory>

namespace ui
{
	class MenuItem;

	class IWindow
	{
	public:
		typedef std::shared_ptr< IWindow > ptr;

		virtual ~IWindow() {}

		virtual IWindow * GetParent() const = 0;
		virtual HWND GetParentHandle() const = 0;	
		virtual HWND GetHandle() const = 0;

		virtual HINSTANCE GetInstance() const = 0;

		virtual IControl* GetControl( int controlID ) const = 0;
		virtual IControl* GetControl( std::string name ) const = 0;

		virtual Menu* GetMenu( HMENU handle ) = 0;
		virtual MenuItem* GetMenuItem( int id ) = 0;
		virtual MenuItem* GetMenuItem( std::string name ) = 0;

	public: // WinApi functions...
		virtual void Close() = 0;
		virtual void Destroy() = 0;
		virtual void GetWindowRect( RECT & rect ) const = 0;
		virtual void MoveWindow( int x, int y, bool repaint ) = 0;
		virtual void ShowWindow( int nCmdShow ) = 0;
		virtual void SetForegroundWindow() = 0;
		virtual void SetText( std::string ) = 0;
		virtual std::string GetText() const = 0;
		virtual int SendUserMessage( int message, message::Params params ) = 0;
		virtual UINT_PTR SetTimer( UINT_PTR id, unsigned int elapsedInMS ) = 0;
		virtual bool KillTimer( UINT_PTR id ) = 0;

	public: // Events...			
		virtual IResult* OnCreate( message::Params params ) = 0;
		virtual IResult* OnClose( message::Params params ) = 0;
		virtual IResult* OnDestroy( message::Params params ) = 0;
		virtual IResult* OnInitDialog( message::Params params ) = 0;
		virtual IResult* OnAfterCreate( message::Params params ) = 0;
		virtual IResult* OnPaint( message::Params params ) = 0;
		virtual IResult* OnControlCommand( message::ControlCommand message ) = 0;
		virtual IResult* OnUser( message::User message ) = 0;
		virtual IResult* OnNotify( message::Notify message ) = 0;
		virtual IResult* OnTimer( message::Timer message ) = 0;
		virtual IResult* OnMenuSelect( message::MenuSelect message ) = 0;
		virtual IResult* OnMenuCommand( message::MenuCommand message ) = 0;
	};
}