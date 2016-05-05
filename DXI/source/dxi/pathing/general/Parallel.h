// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <dxi/pathing/Modifier.h>	

namespace dxi
{
	namespace pathing
	{
		namespace general
		{
			class Parallel : public Modifier
			{
			public:
				typedef std::vector< Modifier::shared_ptr > vector;
				Parallel( std::vector< Modifier::shared_ptr > modifiers );
				void AtTime( float unitTime, unify::Frame * frame );
			private:
				vector m_modifiers;
			};
			}
	}
}