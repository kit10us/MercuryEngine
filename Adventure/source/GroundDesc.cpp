// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <GroundDesc.h>


using namespace me;
using namespace scene;

GroundDesc::GroundDesc()
{
}

GroundDesc::~GroundDesc()
{
}

void GroundDesc::AddDefault( Geometry::ptr geometry )
{
	m_defaults.push_back( geometry );
}

Geometry::ptr GroundDesc::GetDefault( size_t i )
{
	if ( i >= m_defaults.size() )
	{
		return Geometry::ptr();
	}
	return m_defaults[ i ];
}
