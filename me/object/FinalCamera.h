// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved
#pragma once

#include <list>

namespace me
{
	namespace object
	{
		class Object;
		namespace component
		{
			class CameraComponent;
		}

		struct FinalCamera
		{
			Object * object;
			component::CameraComponent * camera;
		};
		typedef std::list< FinalCamera > CameraCache;

	}
}