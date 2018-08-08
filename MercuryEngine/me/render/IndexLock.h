// Copyright (c) 2002 - 2018, Evil Quail LLC
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