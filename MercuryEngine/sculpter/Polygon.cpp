// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <sculpter/Polygon.h>

using namespace sculpter;

Polygon::Polygon( Point origin )
	: m_pointTable{ { "origin", origin } }
{
}

Polygon::~Polygon()
{
}
