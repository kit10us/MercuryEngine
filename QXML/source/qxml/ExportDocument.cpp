// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <qxml/ExportDocument.h>

using namespace qxml;

ExportDocument::ExportDocument()
: m_uTabSize( 3 )
, m_uDepth( 0 )
, m_bInElement( false )
, m_uWritenAttributes( 0 )
{
}

ExportDocument::~ExportDocument()
{
	m_filename.clear();

	unsigned int uElement;
	for( uElement = 0; uElement < MAX_ELEMENT_DEPTH; uElement++ )
	{
		m_elementNameTree[ uElement ].clear();
	}
}

bool ExportDocument::StartSave( const std::string & filename, unsigned int uTabSize )
{
	if( ! m_Stream.Open( unify::StreamAccessType::STREAMACCESS_WRITETEXT, (void *)filename.c_str() ) )
	{
		return false;
	}

	// Leave xml version tag
	if( ! m_Stream.WritePack( (void*)"<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" ) )
	{
		m_Stream.Close();
		return false;
	}

	m_uTabSize = uTabSize;
	return true;
}

bool ExportDocument::EndSave()
{
	// Close all open elements...
	while( m_uDepth )
	{
		if( ! ExportElementEnd() ) return false;
	}
	m_Stream.Close();

	return true;
}

bool ExportDocument::Indent( unsigned int uSpaces )
{
	unsigned int uSpace;
	for( uSpace = 0; uSpace < uSpaces; uSpace++ )
	{
		if( !m_Stream.WritePack( " " ) )
		{
			return false;
		}
	}
	return true;
}

bool ExportDocument::ExportComment( const std::string & comment )
{
	if( m_bInElement ) 
	{
		ExportElementDataStart();
	}

	// Begin comment...
	if( ! Indent( m_uDepth * m_uTabSize ) )
	{
		return false;
	}
	if( ! m_Stream.WritePack( (void*)"<!-- " ) )
	{
		return false;
	}

	unsigned int uChar;
	char szOut[ 2 ];
	szOut[ 1 ] = 0;
	for( uChar = 0; uChar < (unsigned int)comment.size(); ++uChar )
	{
		szOut[ 0 ] = comment[ uChar ];

		if( ! m_Stream.WritePack( (void*)szOut ) )
		{
			return false;
		}

		// Check for new line to indent
		if( szOut[ 0 ] == '\n' )
		{
			if( ! Indent( m_uDepth * m_uTabSize ) )
			{
				return false;
			}
		}

	}

	// End comment
	if( ! m_Stream.WritePack( (void *)" -->\n" ) )
	{
		return false;
	}

	return true;
}

bool ExportDocument::ExportNewline()
{
	// Begin comment...
	if( !m_Stream.WritePack( (void*)"\n" ) )
	{
		return false;
	}

	return true;
}

bool ExportDocument::ExportElementStart( const std::string & name )
{
	// If in an element definition, start the data segment
	if( m_bInElement )
	{
		ExportElementDataStart();
	}

	if( ! Indent( m_uDepth * m_uTabSize ) )
	{
		return false;
	}

	if( ! m_Stream.WritePack( "<" ) ) 
	{
		return false;
	}
	if( ! m_Stream.WritePack( name.c_str() ) )
	{
		return false;
	}

	m_elementNameTree[ m_uDepth++ ] = name;
	m_bInElement = true;
	return true;
}

bool ExportDocument::ExportElementAttribute( const std::string & attributeName, const std::string & attributeValue )
{
	if( ! m_bInElement ) return false;
	if( ! m_Stream.WritePack( " " ) ) return false;
	if( ! m_Stream.WritePack( attributeName.c_str() ) ) return false;
	if( ! m_Stream.WritePack( "=\"" ) ) return false;
	if( ! m_Stream.WritePack( attributeValue.c_str() ) ) return false;
	if( ! m_Stream.WritePack( "\"" ) ) return false;

	++m_uWritenAttributes;
	return true;
}

bool ExportDocument::ExportElementDataStart()
{
	if( !m_bInElement ) return false;
	m_bInElement = false;
	if( !m_Stream.WritePack( ">\n" ) ) return false;

	return true;
}

bool ExportDocument::ExportElementEnd()
{
	if( m_bInElement )
	{
		if( !m_Stream.WritePack( "/>\n" ) ) return false;
		m_bInElement = false;
	}
	else
	{
		if( !Indent( (m_uDepth - 1) * m_uTabSize ) ) return false;
		if( !m_Stream.WritePack( "</" ) ) return false;
		if( !m_Stream.WritePack( m_elementNameTree[ m_uDepth - 1 ].c_str() ) ) return false;
		if( !m_Stream.WritePack( ">\n" ) ) return false;
	}

	m_elementNameTree[ m_uDepth - 1 ].clear();
	
	m_uDepth--;
	return true;
}

bool ExportDocument::ExportElementData( const std::string & data )
{
	if( m_bInElement )
	{
		if( ! ExportElementDataStart() ) 
		{
			return false;
		}
	}

	if( ! Indent( m_uDepth * m_uTabSize ) ) 
	{
		return false;
	}

	unsigned int uChar;
	char szOut[ 2 ];
	szOut[ 1 ] = 0;
	for( uChar = 0; uChar < (unsigned int)data.size(); ++uChar )
	{
		szOut[ 0 ] = data[ uChar ];

		if( ! m_Stream.WritePack( (void *)szOut ) )
		{
			return false;
		}

		// Check for new line to indent
		if( szOut[ 0 ] == '\n' && uChar < (unsigned int)data.size() - 1 )
		{
			if( ! Indent( m_uDepth * m_uTabSize ) )
			{
				return false;
			}
		}

	}

	return true;
}
