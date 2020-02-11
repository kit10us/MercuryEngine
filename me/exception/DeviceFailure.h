// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <unify/Exception.h>

namespace me
{
	namespace exception
	{
		class DeviceFailure : public unify::Exception
		{
		public:
			DeviceFailure( std::string deviceName, std::string error )
				: unify::Exception( "Device \"" + deviceName + "\" failure: " + error )
			{
			}
		};
	} 
}