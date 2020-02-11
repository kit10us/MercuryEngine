// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <qxml/Document.h>
#include <unify/Frame.h>
#include <unify/TexCoords.h>
#include <unify/ColorUnit.h>

namespace me
{
	bool XMLConvert( const qxml::Element * element, unify::V3< float > & v3, std::string postFix = std::string() );
	bool XMLConvert( const qxml::Element * element, unify::Quaternion & q, std::string postFix = std::string() );
	bool XMLConvert( const qxml::Element * element, unify::Matrix & matrix, std::string postFix = std::string() );
	bool XMLConvert( const qxml::Element * element, unify::TexCoords & texCoords, std::string postFix = std::string() );
	bool XMLConvert( const qxml::Element * element, unify::ColorUnit & color, std::string postFix = std::string() );
}