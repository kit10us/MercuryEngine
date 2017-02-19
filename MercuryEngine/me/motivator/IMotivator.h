														 // Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

namespace me
{
	/// <summary>
	/// A motivator allows controllers to interact with objects. Controllers can be players, or paths, or any other
	/// source of input/motivation/force.
	/// </summary>
	class IMotivator
	{
	public:
		virtual ~IMotivator() {}
	};
}