// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <unify/Exception.h>

namespace me::exception
{
	/// <summary>
	/// A hardware device failure.
	/// </summary>
	class DeviceFailure : public unify::Exception
	{
	public:
		DeviceFailure( std::string deviceName, std::string error )
			: unify::Exception( "Device \"" + deviceName + "\" failure: " + error )
		{
		}
	};
}