// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#include <unify/TexArea.h>
#include <unify/Size.h>
#include <unify/String.h>

using namespace unify;

TexArea TexArea::One()
{
	return TexArea( 0, 0, 1, 1 );
}

TexArea TexArea::Zero()
{
	return TexArea( 0, 0, 0, 0 );
}

TexArea TexArea::Full()
{
	return TexArea( 0, 0, 1, 1 );
}

TexArea TexArea::Double()
{
	return TexArea( 0, 0, 2, 2 );
}

TexArea::TexArea()
{
	// Do nothing
}

TexArea::TexArea( float ul_u, float ul_v, float dr_u, float dr_v )
: ul( ul_u, ul_v )
, dr( dr_u, dr_v )
{
}

TexArea::TexArea( const TexCoords & coordsUL, const TexCoords & coordsDR )
: ul( coordsUL )
, dr( coordsDR )
{
}

TexArea::TexArea( const std::string text )
{
    // Named
    if ( StringIs( text, "full" ) )
    {
        ul.u = 0.0f;
        ul.v = 0.0f;
        dr.u = 1.0f;
        dr.v = 1.0f;
    }
	else
	{
		std::vector< float > split = Split< float >( text, ',' );
		ul.u = split[ 0 ];
		ul.v = split[ 1 ];
		dr.u = split[ 2 ];
		dr.v = split[ 3 ];
	}
}

bool TexArea::operator==( const TexArea & area ) const
{
	return ul == area.ul && dr == area.dr;
}

bool TexArea::operator!=( const TexArea & area ) const
{
	return !(*this == area);
}

float TexArea::Width() const
{
	return dr.u - ul.u;
}

float TexArea::Height() const
{
	return dr.v - ul.v;
}

Size< float > TexArea::Size() const
{

    return unify::Size< float >( dr.u - ul.u, dr.v - ul.v );
}

TexCoords TexArea::UL() const
{
	return ul;
}

TexCoords TexArea::UR() const
{
	return TexCoords( dr.u, ul.v );
}

TexCoords TexArea::DL() const
{
	return TexCoords( ul.u, dr.v );
}

TexCoords TexArea::DR() const
{
	return dr;
}
