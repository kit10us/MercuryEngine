// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dae/Accessor.h>

using namespace dxi;
using namespace dae;

Accessor::Accessor( const qxml::Element * node )
: m_count( node->GetIntegerAttribute( "count" ) )
, m_offset( node->GetIntegerAttributeElse( "offset", 0 ) )
, m_source( node->GetStringAttribute( "source" ) )
, m_stride( node->GetIntegerAttributeElse( "stride", 1 ) )
{
}

int Accessor::GetCount() const
{
	return m_count;
}

int Accessor::GetOffset() const
{
	return m_offset;
}

const std::string & Accessor::GetSource()
{
	return m_source;
}

int Accessor::GetStride() const
{
	return m_stride;
}
