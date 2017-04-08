// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <unify/Exception.h>
#include <string>

namespace dae
{
	class Exception_MissingCOLLADA : public unify::Exception
	{
	public:
		Exception_MissingCOLLADA( const std::string & filename );
	};

	class Exception_NotSupported : public unify::Exception
	{
	public:
		Exception_NotSupported( const std::string & feature );
	};

	class Exception_MissingChildElement : public unify::Exception
	{
	public:
		Exception_MissingChildElement( const std::string & parent, const std::string & child );
	};
}
