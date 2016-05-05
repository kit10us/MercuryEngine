// Copyright (c) 2002 - 2011 Quentin S. Smith
// All Rights Reserved

#include <dxi/utils/SafeRelease.h>

void dxi::utils::SafeRelease( IUnknown * unknown )
{
	unknown->Release();
}