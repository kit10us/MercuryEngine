// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <MELuaDLL.h>
#include <lua.hpp>
#include <qxml/Document.h>

namespace melua
{
	class XMLElementProxy
	{
	public:
		MELUADLL_API static char* Name();

		XMLElementProxy( qxml::Element* element );
		XMLElementProxy( qxml::Element::shared_ptr element );

		qxml::Element* GetElement();

	private:
		qxml::Element::shared_ptr m_managed;
		qxml::Element* m_element;
	};

	void RegisterXMLElement( lua_State * state );
}
