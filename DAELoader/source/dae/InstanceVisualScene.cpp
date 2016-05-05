// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#include <dae/InstanceVisualScene.h>

using namespace dxi;
using namespace dae;

InstanceVisualScene::InstanceVisualScene( const qxml::Element * node )
: m_sid( node->GetStringAttributeElse( "sid", "" ) )
, m_name( node->GetStringAttributeElse( "name", "" ) )
, m_url( node->GetStringAttribute( "url" ) )
{
}

std::string InstanceVisualScene::GetSID() const
{
	return m_sid;
}

std::string InstanceVisualScene::GetName() const
{
	return m_name;
}

std::string InstanceVisualScene::GetURL() const
{
	return m_url;
}
