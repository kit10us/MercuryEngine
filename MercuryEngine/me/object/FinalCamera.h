// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <list>

namespace me
{
	namespace object
	{
		struct FinalCamera
		{
			class Object * object;
			class CameraComponent * camera;
		};
		typedef std::list< FinalCamera > CameraCache;

	}
}