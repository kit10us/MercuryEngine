#include <dae/FloatArray.h>
#include <unify/String.h>

using namespace dxi;
using namespace dae;

FloatArray::FloatArray( const qxml::Element * node )
: m_count( node->GetIntegerAttribute( "count" ) )
, m_id( node->GetStringAttributeElse( "id", std::string() ) )
, m_name( node->GetStringAttributeElse( "name", std::string() ) )
, m_digits( node->GetIntegerAttributeElse( "digits", 6 ) )
, m_magnitude( node->GetIntegerAttributeElse( "magnitude", 38 ) )
{
	m_stringContents = node->GetText();
	m_arrayContents = unify::SplitOnWhitespace< float >( m_stringContents );
}

int FloatArray::GetCount() const
{
	return m_count;
}

const std::string & FloatArray::GetID() const
{
	return m_id;
}

const std::string & FloatArray::GetName() const
{
	return m_name;
}

int FloatArray::GetDigits() const
{
	return m_digits;
}

int FloatArray::GetMagnitude() const
{
	return m_magnitude;
}

const std::string & FloatArray::GetStringContents() const
{
	return m_stringContents;
}

const std::vector< float > & FloatArray::GetArrayContents() const
{
	return m_arrayContents;
}
