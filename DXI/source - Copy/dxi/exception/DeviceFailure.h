// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <unify/Exception.h>
#include <string>

namespace dxi
{
	namespace exception
	{
		class DeviceFailure : public unify::Exception
		{
		public:
			DeviceFailure( const std::string & deviceName, const std::string & error )
				: unify::Exception( "Device \"" + deviceName + "\" failure: " + error )
			{
			}
		};
	} // namespace exception
} // namespace dxi