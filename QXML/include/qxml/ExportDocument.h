// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <unify/FStream.h>
#include <unify/LinkList.h>
#include <string>

namespace qxml 
{
	#define MAX_ELEMENT_DEPTH		32

	class ExportDocument
	{
	public:
		ExportDocument();
		~ExportDocument();

		bool StartSave( const std::string & filename, unsigned int uTabSize = 3 );
		bool EndSave();

		bool ExportComment( const std::string & comment );
		bool ExportNewline();

		bool ExportElementStart( const std::string & name );
		bool ExportElementAttribute( const std::string & attributeName, const std::string & attributeValue );
		bool ExportElementDataStart();
		bool ExportElementData( const std::string & data );
		bool ExportElementEnd();

	protected:
		bool Indent( unsigned int uSpaces );

	protected:
		unify::FileStream m_Stream;
		unsigned int m_uDepth;
		unsigned int m_uTabSize;
		std::string m_filename;

		bool m_bInElement;
		unsigned int m_uWritenAttributes;

		std::string m_elementNameTree[ MAX_ELEMENT_DEPTH ];
	};

}
