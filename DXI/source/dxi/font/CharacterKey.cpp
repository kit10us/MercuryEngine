// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#include <dxi/font/CharacterKey.h>

using namespace dxi;
using namespace font;

CharacterKey::CharacterKey()
{
}

CharacterKey::CharacterKey( char keyFromChar )
{
	m_stringKey = keyFromChar;
}

CharacterKey::CharacterKey( const std::string & keyFromString )
{
	m_stringKey = keyFromString;
}

CharacterKey::~CharacterKey() throw ()
{
}

bool CharacterKey::operator==( const CharacterKey & key ) const
{
	return m_stringKey == key.m_stringKey;
}

bool CharacterKey::operator<( const CharacterKey & key ) const
{
	return m_stringKey < key.m_stringKey;
}

bool CharacterKey::empty() const
{
	return m_stringKey.empty();
}

