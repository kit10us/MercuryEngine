// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#pragma comment(lib,"qxml.lib")

#include <qxml/Attribute.h>
#include <qxml/Element.h>
#include <unify/Path.h>
#include <unify/FStream.h>
#include <unify/Exception.h>
#include <unify/unify.h>
#include <vector>

namespace qxml
{
	class Document
	{
	public:
		Document();
		Document( const unify::Path & filePath );
		virtual ~Document();
		virtual void Load( const unify::Path & filePath );
		void Destroy();
		Element * GetRoot();
		Element * GetElement( unsigned int element );
		Element * FindElement( const std::string & element );
		Element * FindElement( const std::string & element, const std::string & attribute, const std::string & value );
		void FindElementsByTagNameRecursive( ElementList & elementList, const std::string & tagName );
		void FindElementsByTagName( ElementList & elementList, const std::string & tagName );
		unsigned int CountElements( const std::string & element );
		unsigned int NumElements();

        const unify::Path & GetPath() const;

	protected:
		Element * AddElement( Element * element, const std::string & name );

		// Elements stored as a list internally, however they keep linkage hierarchicly.
		ElementList m_elementList;
		Element * m_xml; // The XML header node.
		Element * m_root;

		unsigned int m_numChildren;

		 unify::Path m_filePath;
	};
}

