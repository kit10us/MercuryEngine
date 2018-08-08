// Copyright (c) 2002 - 2013, Evil Quail LLC
// All Rights Reserved

#pragma once

template< typename T >
void Releaser( T * thing )
{
	thing->release();
}