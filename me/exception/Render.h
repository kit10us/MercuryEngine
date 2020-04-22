// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <unify/Exception.h>

namespace me::exception
{
	/// <summary>
	/// There was a failure during the rendering process.
	/// </summary>
	class Render : public unify::Exception
	{
	public:
		Render( std::string error )
			: unify::Exception( error )
		{
		}
	};
}