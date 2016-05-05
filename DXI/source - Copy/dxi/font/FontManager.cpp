// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#include <dxi/font/FontManager.h>
#include <qxml/Document.h>

using namespace dxi;
using namespace font;

FontManager::FontManager( Managers managers, animation::SpriteManager::shared_ptr spriteManager )
: m_managers( managers )
, m_spriteManager( spriteManager )
{
}

FontManager::~FontManager() throw ()
{
}

void FontManager::LoadFromFile( const FontName & name, const unify::Path & filePath )
{
	m_fonts[ name ] = std::shared_ptr< Font >( new Font( filePath, m_managers, m_spriteManager.lock() ) ); 
}

void FontManager::AddFont( const FontName & name, std::shared_ptr< Font > font )
{
	m_fonts[ name ] = font;
}

void FontManager::RemoveFont( const FontName & name )
{
	m_fonts.erase( name );
}

bool FontManager::FontExists( const FontName & name ) const
{
	FontMap::const_iterator itrFont = m_fonts.find( name );
	return ( itrFont == m_fonts.end() ) ? false : true;
}

const std::shared_ptr< Font > & FontManager::FindFont( const FontName & name ) const
{
	FontMap::const_iterator itrFont = m_fonts.find( name );
	if( itrFont == m_fonts.end() )
	{
		throw unify::Exception( "Font not found!" );
	}
	else
	{
		return itrFont->second;
	}
}

const FontName & FontManager::GetFontName( const unsigned int index ) const
{
	if( index >= Count() )
	{
		throw unify::Exception( "Font index out of bounds!" );
	}

	unsigned int currentIndex = 0;
	FontMap::const_iterator itrFont = m_fonts.begin();
	for( itrFont; itrFont != m_fonts.end() && currentIndex != index; ++itrFont, ++currentIndex )
	{
		// empty.
	}
	return itrFont->first;
}

unsigned int FontManager::Count() const
{
	return static_cast< unsigned int >( m_fonts.size() );
}

