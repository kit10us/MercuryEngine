// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#include <dxi/font/FontString.h>

using namespace dxi;
using namespace font;

FontString::FontString()
{
}

FontString::FontString( const char * string )
: m_string( string )
{
}

FontString::FontString( const std::string & string )
: m_string( string )
{
}

FontString::~FontString()
{
}

unsigned int FontString::Size() const
{
	return m_string.size();
}

bool FontString::Empty() const
{
	return m_string.empty();
}

CharacterKey FontString::GetKey( unsigned int index ) const
{
	return m_string[ index ];
}
