// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#include <dxi/dae/library_visual_scenes/DAE_InstanceEffect.h>

using namespace dxi;
using namespace dae;

InstanceEffect::InstanceEffect( const qxml::Element * node )
: m_sid( node->GetStringAttributeElse( "sid", std::string() ) )
, m_name( node->GetStringAttributeElse( "name", std::string() ) )
, m_url( node->GetStringAttribute( "url" ) )
{
}

std::string InstanceEffect::GetSID() const
{
	return m_sid;
}

std::string InstanceEffect::GetName() const
{
	return m_name;
}

std::string InstanceEffect::GetURL() const
{
	return m_url;
}
