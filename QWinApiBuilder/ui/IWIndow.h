// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <ui/POD.h>
#include <memory>

namespace ui
{
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

	public: // WinApi functions...
		virtual void GetWindowRect( RECT & rect ) const = 0;
		virtual void MoveWindow( int x, int y, bool repaint ) = 0;
		virtual void ShowWindow( int nCmdShow ) = 0;
		virtual void SetForegroundWindow() = 0;
		virtual void SetText( std::string ) = 0;
		virtual std::string GetText() const = 0;
		virtual int SendUserMessage( int message, Params params ) = 0;
		virtual UINT_PTR SetTimer( UINT_PTR id, unsigned int elapsedInMS ) = 0;
		virtual bool KillTimer( UINT_PTR id ) = 0;

	public: // Events...			
		virtual IResult* OnCreate( Params params ) = 0;
		virtual IResult* OnDestroy( Params params ) = 0;
		virtual IResult* OnInitDialog( Params params ) = 0;
		virtual IResult* OnAfterCreate( Params params ) = 0;
		virtual IResult* OnPaint( Params params ) = 0;
		virtual IResult* OnControlCommand( ControlMessage message ) = 0;
		virtual IResult* OnUserMessage( UserMessageData message ) = 0;
		virtual IResult* OnNotify( NotifyMessage message ) = 0;
		virtual IResult* OnTimer( TimerMessage message ) = 0;
	};
}