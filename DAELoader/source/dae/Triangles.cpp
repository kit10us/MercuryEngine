#include <dae/Triangles.h>
#include <unify/String.h>

using namespace dxi;
using namespace dae;

Triangles::Triangles( IDocument & document, const qxml::Element * node )
: DocumentNode( document, node )
, m_name( node->GetStringAttributeElse( "name", std::string() ) )
, m_count( node->GetIntegerAttribute( "count" ) )
, m_material( node->GetStringAttributeElse( "material", std::string() ) )
, m_vertexFormat()
{
	for ( const qxml::Element * childNode = node->GetFirstChild(); childNode; childNode = childNode->GetNext() )
	{
		if ( childNode->IsTagName( "input" ) )
		{
			m_input.push_back( std::shared_ptr< Input_Shared >( new Input_Shared( childNode ) ) );
		}
		else if ( childNode->IsTagName( "p" ) )
		{
			m_p = unify::SplitOnWhitespace< int >( childNode->GetText() );
		}
	}

	qjson::Object format;
	for( auto input : m_input )
	{
		format.Add( { input->GetSemantic(), input->GetSemantic() } );
	}
	m_vertexFormat = VertexDeclaration( format );
}

const std::string & Triangles::GetName() const
{
	return m_name;
}

int Triangles::GetCount() const
{
	return m_count;
}

const std::string & Triangles::GetMaterial() const
{
	return m_material;
}

const std::vector< std::shared_ptr< Input_Shared > > & Triangles::GetInput() const
{
	return m_input;
}

const std::vector< int > & Triangles::GetP() const
{
	return m_p;
}

dxi::VertexDeclaration Triangles::GetVertexFormat() const
{
	return m_vertexFormat;
}
