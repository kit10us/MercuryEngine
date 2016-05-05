// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved
#include <dae/Technique_Core.h>

using namespace dxi;
using namespace dae;

Technique_Core::Technique_Core( const qxml::Element * node )
: m_xmlNode( node )
, m_profile( node->GetStringAttribute( "profile" ) )
{
}

const std::string & Technique_Core::GetProfile() const
{
	return m_profile;
}

const qxml::Element & Technique_Core::GetXML() const
{
	return *m_xmlNode;
}