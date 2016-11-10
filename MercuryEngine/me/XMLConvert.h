// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <qxml/Document.h>
#include <unify/Frame.h>
#include <unify/TexCoords.h>
#include <unify/ColorUnit.h>

namespace me
{
	bool XMLConvert( const qxml::Element * element, unify::V3< float > & v3, const std::string & postFix = std::string() );
	bool XMLConvert( const qxml::Element * element, unify::Quaternion & q, const std::string & postFix = std::string() );
	bool XMLConvert( const qxml::Element * element, unify::Matrix & matrix, const std::string & postFix = std::string() );
	bool XMLConvert( const qxml::Element * element, unify::TexCoords & texCoords, const std::string & postFix = std::string() );
	bool XMLConvert( const qxml::Element * element, unify::ColorUnit & color, const std::string & postFix = std::string() );
}