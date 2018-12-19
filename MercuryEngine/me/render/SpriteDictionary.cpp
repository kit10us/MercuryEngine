// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <me/render/SpriteDictionary.h>
#include <me/exception/NotImplemented.h>
#include <me/exception/FailedToLock.h>
#include <qxml/Document.h>
#include <unify/RowColumn.h>
#include <unify/Parameters.h>
#include <cassert>

using namespace me;
using namespace render;

SpriteDictionary::SpriteDictionary()
	: m_asciiNumeric( false )
	, m_asciiLowercase( false )
	, m_asciiUppercase( false )
	, m_asciiNull( unify::TexAreaZero() )
{
}

SpriteDictionary::~SpriteDictionary()
{
}

void SpriteDictionary::LoadDictionary( unify::Path source )
{
	qxml::Document document;
	document.Load( source );

	unify::Size< float > imagesize;

	for ( const auto node : document.GetRoot()->Children() )
	{
		if ( node.IsTagName( "imagesize" ) )
		{
			imagesize.width = node.GetAttribute< float >( "width" );
			imagesize.height = node.GetAttribute< float >( "height" );
		}

		if ( node.IsTagName( "addsheet" ) )
		{
			std::string name = node.GetAttributeElse< std::string >( "name", "" );
			unify::TexCoords arrayUL( node.GetAttributeElse< float >( "ulu", 0 ), node.GetAttributeElse< float >( "ulv", 0 ) );
			unify::Size< float > size;
			unify::RowColumn< unsigned int > arrayRC{ qxml::AttributeCast< unify::RowColumn< unsigned int >, unsigned int >( node, "r", "c" ) };
			assert( arrayRC.row && arrayRC.column );
						
			if ( node.HasAttributes( "sizeu,sizev" ) )
			{
				size.width = node.GetAttribute( "sizeu" )->Get< float >();
				size.height = node.GetAttribute( "sizev" )->Get< float >();
			}
			else
			{
				size = unify::Size< float >( (1 - arrayUL.u) / arrayRC.column, (1 - arrayUL.v) / arrayRC.column );
			}

			if ( !name.empty() )
			{
				unsigned int head = static_cast< unsigned int >(m_spriteMasterList.size());
				m_spriteArrayMap[name] = SpriteArray( head, head + arrayRC.row * arrayRC.column );
			}

			unify::RowColumn< unsigned int > rc;
			for ( rc.row = 0; rc.row < arrayRC.row; ++rc.row )
			{
				for ( rc.column = 0; rc.column < arrayRC.column; ++rc.column )
				{
					unify::TexCoords ul( arrayUL.u + rc.column * size.width, arrayUL.v + rc.row * size.height );
					unify::TexCoords dr( ul + unify::TexCoords( size.width, size.height ) );
					m_spriteMasterList.push_back( unify::TexArea( ul, dr ) );
				}
			}
		}
		else if ( node.IsTagName( "addascii" ) )
		{
			unify::TexArea area;

			std::string name = node.GetAttribute< std::string >( "name" );

			if ( node.HasAttributes( "ulx" ) )
			{
				area.ul.u = node.GetAttribute< float >( "ulx" ) / imagesize.width;
			}

			if ( node.HasAttributes( "uly" ) )
			{
				area.ul.v = node.GetAttribute< float >( "uly" ) / imagesize.height;
			}

			if ( node.HasAttributes( "drx" ) )
			{
				area.dr.u = node.GetAttribute< float >( "drx" ) / imagesize.width;
			}

			if ( node.HasAttributes( "dry" ) )
			{
				area.dr.v = node.GetAttribute< float >( "dry" ) / imagesize.height;
			}		   

			if ( node.HasAttributes( "ulu" ) )
			{
				area.ul.u = node.GetAttribute< float >( "ulu" );
			}

			if ( node.HasAttributes( "ulv" ) )
			{
				area.ul.v = node.GetAttribute< float >( "ulv" );
			}
					  
			if ( node.HasAttributes( "sizeu" ) )
			{
				area.dr.u = area.ul.u + node.GetAttribute( "sizeu" )->Get< float >();
			}

			if ( node.HasAttributes( "sizeu" ) )
			{
				area.dr.v = area.ul.v + node.GetAttribute( "sizev" )->Get< float >();
			}


			if ( unify::string::StringIs( name, "null" ) )
			{
				m_asciiNull = area;
			}
			else
			{
				char ascii = name[ 0 ];
				m_ascii[ ascii ] = area;
			}
		}
	}

	// Test for ascii sets...

	m_asciiNumeric = true;
	for( char c = '0'; c <= '9'; ++c )
	{				
		const auto itr = m_ascii.find( c );
		if ( itr == m_ascii.end() )
		{
			m_asciiNumeric = false;
			break;
		}
	}

	m_asciiLowercase = true;
	for( char c = 'a'; c <= 'z'; ++c )
	{				
		const auto itr = m_ascii.find( c );
		if ( itr == m_ascii.end() )
		{
			m_asciiLowercase = false;
			break;
		}
	}

	m_asciiUppercase = true;
	for( char c = 'A'; c <= 'Z'; ++c )
	{				
		const auto itr = m_ascii.find( c );
		if ( itr == m_ascii.end() )
		{
			m_asciiUppercase = false;
			break;
		}
	}
}

bool SpriteDictionary::HasSpriteArray( std::string name ) const
{
	SpriteArrayMap::const_iterator itr = m_spriteArrayMap.find( name );
	return itr == m_spriteArrayMap.end() ? false : true;
}

const SpriteDictionary::SpriteArray & SpriteDictionary::FindSpriteArray( std::string name ) const
{
	SpriteArrayMap::const_iterator itr = m_spriteArrayMap.find( name );
	if ( itr == m_spriteArrayMap.end() )
	{
		throw unify::Exception( "Sprite array not found!" );
	}
	return itr->second;
}

const SpriteDictionary::SpriteArray & SpriteDictionary::GetSpriteArray( unsigned int index ) const
{
	assert( index < m_spriteArrayMap.size() );

	unsigned int currentIndex = 0;
	SpriteArrayMap::const_iterator itr = m_spriteArrayMap.begin();
	for ( itr; itr != m_spriteArrayMap.end() && currentIndex != index; ++itr, ++currentIndex )
	{
		// Empty.
	}
	return itr->second;
}

unsigned int SpriteDictionary::SpriteArrayCount() const
{
	return static_cast< unsigned int >(m_spriteArrayMap.size());
}

const unify::TexArea & SpriteDictionary::GetSprite( unsigned int index ) const
{
	assert( index < m_spriteMasterList.size() );
	return m_spriteMasterList[index];
}

const unify::TexArea & SpriteDictionary::GetSprite( std::string arrayName, unsigned int index ) const
{
	const SpriteArrayMap::const_iterator itr = m_spriteArrayMap.find( arrayName );
	if ( itr == m_spriteArrayMap.end() )
	{
		throw unify::Exception( "Sprite array not found!" );
	}

	assert( index < itr->second.tail );

	return m_spriteMasterList[itr->second.head + index];
}

unsigned int SpriteDictionary::SpriteCount() const
{
	return static_cast< unsigned int >(m_spriteMasterList.size());
}								  

bool SpriteDictionary::HasAsciiNumeric() const
{
	return m_asciiNumeric;
}

bool SpriteDictionary::HasAsciiLowercase() const
{
	return m_asciiLowercase;
}

bool SpriteDictionary::HasAsciiUppercase() const
{
	return m_asciiUppercase;
}

bool SpriteDictionary::HasAscii( char ascii ) const
{
	const auto itr = m_ascii.find( ascii );
	return ( itr == m_ascii.end() ) ? false : true;
}

unify::TexArea SpriteDictionary::GetAscii( char ascii ) const
{
	const auto itr = m_ascii.find( ascii );
	if ( itr == m_ascii.end() ) return m_asciiNull;

	return itr->second;
}