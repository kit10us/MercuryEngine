// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#include <dxi/dae/library_effects/DAE_Effect.h>

using namespace dxi;
using namespace dae;

dae::Effect::Effect( IDocument & document, const qxml::Element * node )
: DocumentNode( document, node )
, m_id( node->GetStringAttribute( "id" ) )
, m_name( node->GetStringAttributeElse( "name", std::string() ) )
{
	for ( const qxml::Element * childNode = node->GetFirstChild(); childNode; childNode = childNode->GetNext() )
	{
		if ( childNode->IsTagName( "profile_COMMON" ) )
		{
			m_profileCOMMON.reset( new ProfileCOMMON( document, childNode ) );
		}
	}
}

const std::string & dae::Effect::GetID() const
{
	return m_id;
}

const std::string & dae::Effect::GetName() const
{
	return m_name;
}

const ProfileCOMMON * dae::Effect::GetProfileCOMMON() const
{
	return m_profileCOMMON.get();
}
