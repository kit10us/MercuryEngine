// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <me/IThing.h>
#include <unify/Lookup.h>
#include <unify/Cast.h>
#include <string>
#include <memory>
#include <list>
#include <functional>

namespace me
{
	namespace interop
	{
		class IValue
		{
		public:
			typedef std::shared_ptr< IValue > ptr;
			typedef std::shared_ptr< const IValue > const_ptr;

			virtual ~IValue() {}

			virtual bool IsWriteable() const = 0;
			virtual void Set( std::string value ) = 0;
			virtual std::string Get() const = 0;
			virtual std::string ToString() const = 0;
		};
	}
}