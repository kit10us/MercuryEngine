// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved
//

#include <me/render/Filtering.h>
#include <unify/String.h>
#include <unify/Exception.h>

using namespace me;
using namespace render;

std::string me::render::Filtering::ToString( Filtering::TYPE filtering )
{							   
	switch( filtering )
	{
	case Point: return "Point";
	case Linear: return "Linear";
	case Anisotropic: return "Anisotropic";
	default: throw unify::Exception( "Unknown filtering type!" );
	}
}

Filtering::TYPE me::render::Filtering::FromString( std::string filtering )
{
	using namespace unify;
	if ( string::StringIs( filtering, "Point" ) ) return Filtering::Point;
	else if (string::StringIs( filtering, "Linear" ) ) return Filtering::Linear;
	else if (string::StringIs( filtering, "Anisotropic" ) ) return Filtering::Anisotropic;
	else throw Exception( "Unknown filtering string \"" + filtering + "\"!" );
}
