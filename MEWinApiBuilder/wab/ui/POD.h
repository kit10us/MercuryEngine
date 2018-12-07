// Copyright (c) 2002 - 2018, Evil Quail LLC
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

#include <wab/ui/IControl.h>

namespace ui
{
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
}