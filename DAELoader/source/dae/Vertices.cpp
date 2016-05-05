#include <dae/Vertices.h>

using namespace dxi;
using namespace dae;

Vertices::Vertices( IDocument & document, const qxml::Element * node )
: DocumentNode( document, node )
, m_id( node->GetStringAttribute( "id" ) )
, m_name( node->GetStringAttributeElse( "name", std::string() ) )
{
	for ( const qxml::Element * childNode = node->GetFirstChild(); childNode; childNode = childNode->GetNext() )
	{
		if ( childNode->IsTagName( "input" ) )
		{
			m_input.push_back( std::shared_ptr< Input_Unshared >( new Input_Unshared( document, childNode ) ) );
		}
	}
}

const std::string & Vertices::GetID() const
{
	return m_id;
}

const std::string & Vertices::GetName() const
{
	return m_name;
}

const std::vector< std::shared_ptr< Input_Unshared > > & Vertices::GetInput() const
{
	return m_input;
}
