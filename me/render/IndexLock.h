// Copyright (c) 2002 - 2018, Kit10 Studios LLC
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
			void SetIndex( size_t index, size_t value );
			size_t GetIndex( size_t index );
		};
	}
} 