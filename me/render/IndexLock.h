// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <unify/unify.h>
#include <me/util/DataLock.h>

namespace me::render
{
	class IndexLock : public util::DataLock
	{
	public:
		void SetIndex( size_t index, size_t value );
		size_t GetIndex( size_t index );
	};
} 