#include <dae/Source.h>
#include <dae/Document.h>
#include <unify/V3.h>
#include <unify/TexCoords.h>

using namespace dae;

Source::Source( IDocument & document, const qxml::Element * node )
: DocumentNode( document, node )
, m_id( node->GetAttribute< std::string >( "id" ) )
, m_name( node->GetAttributeElse( "name", std::string() ) )
{
	for ( const qxml::Element * childNode = node->GetFirstChild(); childNode; childNode = childNode->GetNext() )
	{
		if ( childNode->IsTagName( "float_array" ) )
		{
			m_float_array.reset( new FloatArray( childNode ) );
		}
		else if ( childNode->IsTagName( "technique_common" ) )
		{
			m_technique_common.reset( new Source_TechniqueCommon( childNode ) );
		}
		else
		{
			throw Exception_NotSupported( childNode->GetTagName() );
		}
	}

	if ( m_float_array && m_technique_common )
	{
		m_sourceType = m_name;
	}
}

const std::string & Source::GetID() const
{
	return m_id;
}

const std::string & Source::GetName() const
{
	return m_name;
}

const FloatArray & Source::GetFloatArray() const
{
	return *m_float_array;
}

const Source_TechniqueCommon & Source::GetTechniqueCommon() const
{
	return *m_technique_common;
}

std::string Source::GetSourceType() const
{
	return m_sourceType;
}