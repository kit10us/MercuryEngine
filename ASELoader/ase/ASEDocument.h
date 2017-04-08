// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <qxml/Document.h>

namespace ase
{
	class Document : public qxml::Document
	{
	public:
		void Load( const unify::Path & filePath );
	};
}