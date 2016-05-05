#pragma once

#include <memory> // For std::tr1::shared_ptr

// Used with shared_ptr to release instead of delete.
template< typename T >
void Releaser( T * thing )
{
	if ( thing )
	{
		thing->Release();
	}
}

