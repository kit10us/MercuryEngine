// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved
//

#include <dxi/Filtering.h>
#include <unify/String.h>
#include <unify/Exception.h>

using namespace dxi;

std::string dxi::Filtering::ToString( Filtering::TYPE filtering )
{							   
	switch( filtering )
	{
	case Point: return "Point";
	case Linear: return "Linear";
	case Anisotropic: return "Anisotropic";
	default: throw unify::Exception( "Unknown filtering type!" );
	}
}

Filtering::TYPE dxi::Filtering::FromString( std::string filtering )
{
	using namespace unify;
	if ( StringIs( filtering, "Point" ) ) return Filtering::Point;
	else if ( StringIs( filtering, "Linear" ) ) return Filtering::Linear;
	else if ( StringIs( filtering, "Anisotropic" ) ) return Filtering::Anisotropic;
	else throw Exception( "Unknown filtering string \"" + filtering + "\"!" );
}
