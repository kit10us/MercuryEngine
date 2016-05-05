// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/Usage.h>
#include <unify/String.h>
#include <unify/Exception.h>

using namespace dxi;

Usage::TYPE Usage::FromString( const std::string text )
{
	std::string trimmedText = unify::CleanWhitespace( text );
    if ( unify::StringIs( trimmedText, "Ignore" ) ) return Usage::Ignore;
    else if ( unify::StringIs( trimmedText, "False" ) ) return Usage::False;
    else if ( unify::StringIs( trimmedText, "True" ) ) return Usage::True;
    else if ( unify::StringIs( trimmedText, "TrueNoSet" ) ) return Usage::TrueNoSet;
    else
    {
        throw unify::Exception( "Invalid string conversion for Usage!" );
    }
}