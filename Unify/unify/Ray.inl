// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

template< typename T >
Ray< T >::Ray()
{
    // POD - no initialization.
}

template< typename T >
Ray< T >::Ray( const V3< T > & originIn, const V3< T > & directionIn )
	: origin( originIn )
	, direction( directionIn )
{
}

template< typename T >
V3< T > Ray< T >::InvDirection() const
{
	return V3< T >::V3Inverse( direction );
}

template< typename T >
V3< int > Ray< T >::Sign() const
{
	V3< float > invDirection = InvDirection();
	return V3< int >( invDirection.x < 0, invDirection.y < 0, invDirection.z < 0 );
}

