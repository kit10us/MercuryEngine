// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/i/IMotivator.h>
#include <me/i/IController.h>

namespace me
{	
	class ObjectMotivator : public i::IMotivator
	{
	public:
		ObjectMotivator()
		{
		}

		~ObjectMotivator()
		{
		}

		void AttachController( i::IController * controlle )
		{
		}
	};
}