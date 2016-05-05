// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#include <qxml/XMLParameters.h>

using namespace qxml;

bool XMLParameters::CaseInsensitiveLessThanTest::operator() ( const std::string & string1, const std::string & string2 ) const
{
    return _stricmp( string1.c_str(), string2.c_str() ) < 0;
}

XMLParameters::XMLParameters()
{
}

XMLParameters::~XMLParameters()
{
}

void XMLParameters::AddConvertor( const std::string name, INodeConvertor::shared_ptr convertor )
{
	m_convertors[ name ] = convertor;
}

bool XMLParameters::Convert( const qxml::Element * element, unify::Parameters & parameters )
{
	ConvertorMap::const_iterator itr = m_convertors.find( element->GetTagName() );

	if ( itr == m_convertors.end() )
	{
		return false;
	}

	return (*itr->second)( element, parameters );
}

size_t XMLParameters::NumberOfConvertors() const
{
	return m_convertors.size();
}
