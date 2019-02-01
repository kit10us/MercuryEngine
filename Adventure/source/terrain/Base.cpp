// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <terrain/Base.h>

using namespace me;
using namespace render;
using namespace object;
using namespace terrain;

Base::Base()
{
}

Base::~Base()
{
}

void Base::AddGeneric( Geometry::ptr geometry )
{
	m_generics.push_back( geometry );
}

Geometry::ptr Base::GetGeneric( size_t i )
{
	if ( i >= m_generics.size() )
	{
		return Geometry::ptr();
	}
	return m_generics[ i ];
}

size_t Base::GetGenericCount() const
{
	return m_generics.size();
}
