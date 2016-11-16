// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/Instancing.h>
#include <unify/Exception.h>
#include <unify/String.h>

using namespace me;

std::string Instancing::ToString( Instancing::TYPE value )
{
	switch( value )
	{
	case Instancing::None:
		return "None";
	case Instancing::QP:
		return "QP";
	case Instancing::Matrix:
		return "Matrix";
	default:
		throw unify::Exception( "Unknown Instacing value!" );
	}
}

Instancing::TYPE Instancing::FromString( std::string value )
{
	if ( unify::StringIs( value, "None" ) )
	{
		return Instancing::None;
	}
	else if ( unify::StringIs( value, "QP" ) )
	{
		return Instancing::QP;
	}
	else if ( unify::StringIs( value, "Matrix" ) )
	{
		return Instancing::Matrix;
	}

	throw unify::Exception( "Unknown Instancing value as string! (" + value + ")" );
}
