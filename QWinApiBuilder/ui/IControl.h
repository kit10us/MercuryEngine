// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <Windows.h>

// Undefine Microsoft clashing defines.
#ifdef CreateWindow
#undef CreateWindow
#endif

#ifdef GetCommandLine
#undef GetCommandLine
#endif

#include <string>
#include <cassert>
#include <memory>

namespace ui
{
	class IControl
	{
	public:
		typedef std::shared_ptr< IControl > ptr;

		virtual ~IControl() {}

		virtual HWND GetHandle() const = 0;
		virtual HWND GetParentHandle() const = 0;

		virtual std::string GetName() const = 0;
		virtual int GetID() const = 0;
		
	public: // WinApi functions...
		virtual void SetText( std::string text ) = 0;
		virtual std::string GetText() const = 0;
		virtual bool GetEnabled() const = 0;
		virtual void SetEnable( bool enable ) = 0;
	};
}