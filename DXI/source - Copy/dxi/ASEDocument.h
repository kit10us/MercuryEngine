// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <qxml/Document.h>

namespace dxi
{
	class ASEDocument : public qxml::Document
	{
	public:
		void Load( const unify::Path & filePath );
	protected:
		void ProcessASEElement( qxml::Element * pElement, const std::string & sString );
	};

}