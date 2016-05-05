// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/exception/Exception.h>
#include <string>

namespace dxi
{
	namespace exception
	{
		class UnsupportedValue : public unify::Exception
		{
		public:
			UnsupportedValue( const std::string & type, const std::string & value )
				: unify::Exception( "Unsupported value \"" + value + "\" of type " + type + "!" )
			{
			}
			UnsupportedValue( const std::string & value )
				: unify::Exception( "Unsupported value \"" + value + "\"!" )
			{
			}
		};
	} // namespace exception
} // namespace dxi