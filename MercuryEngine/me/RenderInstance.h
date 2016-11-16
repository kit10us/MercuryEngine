// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <unify/Matrix.h>
						
namespace me
{
	struct RenderInstance
	{
		RenderInstance( unify::Matrix _m );

		unify::Matrix m;
	};
}