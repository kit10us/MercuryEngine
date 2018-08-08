// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <MELuaDLL.h>
#include <lua.hpp>
#include <qxml/Document.h>

namespace melua
{
	class XMLDocumentProxy
	{
	public:
		MELUADLL_API static char* Name();

		XMLDocumentProxy( qxml::Document* document );
		XMLDocumentProxy( std::shared_ptr< qxml::Document > document );

		qxml::Document* GetDocument();

		~XMLDocumentProxy();

	private:
		qxml::Document* m_document;
		std::shared_ptr< qxml::Document > m_managed;
	};

	void RegisterXMLDocument( lua_State * state );
}
