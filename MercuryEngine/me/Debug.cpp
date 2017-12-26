// Copyright (c) 2003 - 2011, Quentin S. Smith
// All Rights Reserved

#include <me/Debug.h>

using namespace me;

bool Debug::Assert( bool assertion ) const
{
	bool debug =
#ifdef _DEBUG
		debug = true;
#else
		debug = false;
#endif

	assert( assertion );
	// TODO: Proper assertion based on debug or release.
	return false;
}
