#include <dae/Polylist.h>
#include <unify/String.h>

using namespace dxi;
using namespace dae;

Polylist::Polylist( IDocument & document, const qxml::Element * node )
: DocumentNode( document, node )
, m_name( node->GetStringAttributeElse( "name", std::string() ) )
, m_count( node->GetIntegerAttribute( "count" ) )
, m_material( node->GetStringAttributeElse( "material", std::string() ) )
, m_vertexFormat( 0 )
{
	std::vector< char > splitDelimitors;
	splitDelimitors.push_back( ' ' );
	splitDelimitors.push_back( '\n' );
	splitDelimitors.push_back( '\t' );

	if ( node->IsTagName( "polylist" ) )
	{
		m_type = PolylistType;
	}
	else if ( node->IsTagName( "triangles" ) )
	{
		m_type = TrianglesType;
		m_vcount = std::vector< int >( m_count, 3 );
	}
	else if ( node->IsTagName( "lines" ) )
	{
		m_type = LinesType;
		m_vcount = std::vector< int >( m_count, 2 );
	}
	else
	{
		throw unify::Exception( "\"" + node->GetTagName() + "\" is not a supported primitive type for Collada documents!" );
	}

	for ( const qxml::Element * childNode = node->GetFirstChild(); childNode; childNode = childNode->GetNext() )
	{
		if ( childNode->IsTagName( "input" ) )
		{
			m_input.push_back( std::shared_ptr< Input_Shared >( new Input_Shared( childNode ) ) );
		}
		else if ( childNode->IsTagName( "vcount" ) )
		{
			m_vcount = unify::SplitOnWhitespace< int >( childNode->GetText() );
		}
		else if ( childNode->IsTagName( "p" ) )
		{
			m_p = unify::SplitOnWhitespace< int >( childNode->GetText() );
		}
	}

	for( size_t i = 0; i < m_input.size(); ++i )
	{
		const Input_Shared & input = *m_input[ i ];
		m_vertexFormat |= input.GetFVFType();
	}
}

const std::string & Polylist::GetName() const
{
	return m_name;
}

int Polylist::GetCount() const
{
	return m_count;
}

const std::string & Polylist::GetMaterial() const
{
	return m_material;
}

const std::vector< std::shared_ptr< Input_Shared > > & Polylist::GetInput() const
{
	return m_input;
}

const std::vector< int > & Polylist::GetVCount() const
{
	return m_vcount;
}

const std::vector< int > & Polylist::GetP() const
{
	return m_p;
}

VertexFormat Polylist::GetVertexFormat() const
{
	return m_vertexFormat;
}

Polylist::Type Polylist::GetType() const
{
	return m_type;
}
