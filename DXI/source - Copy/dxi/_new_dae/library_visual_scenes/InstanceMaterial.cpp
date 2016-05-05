// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#include <dxi/dae/library_visual_scenes/DAE_InstanceMaterial.h>

using namespace dxi;
using namespace dae;

InstanceMaterial::InstanceMaterial( const qxml::Element * node )
: m_sid( node->GetStringAttributeElse( "sid", std::string() ) )
, m_name( node->GetStringAttributeElse( "name", std::string() ) )
, m_target( node->GetStringAttribute( "target" ) )
, m_symbol( node->GetStringAttribute( "symbol" ) )
{
}

std::string InstanceMaterial::GetSID() const
{
	return m_sid;
}

std::string InstanceMaterial::GetName() const
{
	return m_name;
}

std::string InstanceMaterial::GetTarget() const
{
	return m_target;
}

std::string InstanceMaterial::GetSymbol() const
{
	return m_symbol;
}
