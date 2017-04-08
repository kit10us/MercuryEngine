// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <unify/unify.h>
#include <unify/DataLock.h>

namespace me
{
	namespace render
	{
		class IndexLock : public unify::DataLock
		{
		public:
			void SetIndex( unsigned int index, unsigned int );
			unsigned int GetIndex( unsigned int index );
		};
	}
} 