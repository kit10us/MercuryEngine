// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved
#include <dxi/dae/DAE_Extra.h>

using namespace dxi;
using namespace dae;

Extra::Extra( const qxml::Element * node )
{
	for ( const qxml::Element * childNode = node->GetFirstChild(); childNode; childNode = childNode->GetNext() )
	{
		if ( childNode->IsTagName( "technique" ) )
		{
			boost::shared_ptr< Technique_Core > technique( new Technique_Core( childNode ) );
			m_techniques.push_back( technique );
		}
	}

}

const std::vector< boost::shared_ptr< Technique_Core > > & Extra::GetTechniques() const
{
	return m_techniques;
}
