// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <unify/String.h>
#include <windows.h>
#include <memory>
#include <map>

namespace meedr
{
	namespace ui
	{
		struct Message
		{
			HWND handle;
			UINT message; 
			WPARAM wParam;
			LPARAM lParam;
		};

		struct Params
		{
			WPARAM wParam;
			LPARAM lParam;
		};

		struct ControlMessage
		{
			std::string name;
			int controlId;
			int message;
			HWND controlHandle;

			bool IsFor( std::string _name ) const
			{
				return unify::StringIs( name, _name );
			}
		};

		struct IResult
		{
			typedef std::shared_ptr< IResult > ptr;
			virtual ~IResult() {}
			virtual bool IsUnhandled() const = 0;
			virtual unsigned int GetResult() const = 0;
		};

		struct Unhandled : public IResult
		{
			bool IsUnhandled() const override 
			{
				return true;
			}	 

			unsigned int GetResult() const
			{
				return 0;
			}
		};

		struct Result : public IResult
		{
			Result( unsigned int result ) : result{ result } {}
			unsigned int result;
			bool IsUnhandled() const override
			{
				return false;
			}
			unsigned int GetResult() const
			{
				return result;
			}
		};

		struct UserMessageData
		{	
			int message;
			Params params;
		};

		class IWindow
		{
		public:
			typedef std::shared_ptr< IWindow > ptr;

			virtual ~IWindow() {}

			virtual HWND GetParentHandle() const = 0;

			virtual HWND GetHandle() const = 0;

			virtual HINSTANCE GetInstance() const = 0;

			virtual std::string FindControl( int controlID ) const = 0;
			virtual int GetControl( std::string name ) const = 0;

		public: // WinApi functions...
			virtual void MoveWindow( int x, int y, bool repaint ) = 0;

		public: // Events...			
			virtual IResult* OnCreate( Params params ) = 0;
			virtual IResult* OnDestroy( Params params ) = 0;
			virtual IResult* OnInitDialog( Params params ) = 0;
			virtual IResult* OnAfterCreate( Params params ) = 0;
			virtual IResult* OnPaint( Params params ) = 0;
			virtual IResult* OnControlCommand( ControlMessage message ) = 0;
			virtual IResult* OnUserMessage( UserMessageData message ) = 0;

		protected:
			virtual int RegistryControl( std::string name ) = 0;
		};
	}
}