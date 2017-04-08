// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

template< typename T >
T Lerp( T a, T b, float delta )
{
	return a + (b - a) * delta;
}
