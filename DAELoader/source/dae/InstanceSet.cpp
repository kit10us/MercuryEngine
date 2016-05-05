// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#include <dae/InstanceSet.h>

using namespace dxi;
using namespace dae;

InstanceSet::InstanceSet( const qxml::Element * node, InstanceSet::InstanceType type )
: m_sid( node->GetStringAttributeElse( "sid", "" ) )
, m_name( node->GetStringAttributeElse( "name", "" ) )
, m_url( node->GetStringAttribute( "url" ) )
, m_type( type )
{
}

InstanceSet::~InstanceSet()
{
}

std::string InstanceSet::GetSID() const
{
	return m_sid;
}

std::string InstanceSet::GetName() const
{
	return m_name;
}

std::string InstanceSet::GetURL() const
{
	return m_url;
}

InstanceSet::InstanceType InstanceSet::GetType() const
{
	return m_type;
}