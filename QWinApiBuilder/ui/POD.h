// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <unify/String.h>
#include <windows.h>
#include <memory>

// Undefine Microsoft clashing defines.
#ifdef CreateWindow
#undef CreateWindow
#endif

#ifdef GetCommandLine
#undef GetCommandLine
#endif

#include <ui/IControl.h>

namespace ui
{
	class Menu;

	struct Message
	{
		HWND handle;
		UINT message; 
		WPARAM wParam;
		LPARAM lParam;
	};

	struct Params
	{
		Params() : wParam{ 0 }, lParam{ 0 } {}
		Params( WPARAM w, LPARAM l ) : wParam{ w }, lParam{ l } {}
		WPARAM wParam;
		LPARAM lParam;
	};

	struct ControlMessage
	{
		IControl* control;
		int code;

		bool IsFor( std::string _name ) const
		{
			return unify::StringIs( control->GetName(), _name );
		}
	};

	struct NotifyMessage
	{
		IControl* control;
		unsigned int code;
		LPARAM lParam;

		bool IsFor( std::string _name ) const
		{
			return unify::StringIs( control->GetName(), _name );
		}
	};


					   
	struct TimerMessage
	{
		UINT_PTR id;
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
}