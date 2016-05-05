/* Copyright (c) 2002 - 2014, Quentin S. Smith
 * All Rights Reserved
 */

inline
BSphere::BSphere( float _radius )
: radius( _radius )
{
}


inline
void BSphere::AddBSphere( const BSphere & sphere )
{
	if( sphere.radius > radius )
	{
		radius = sphere.radius;
	}
}

inline
void BSphere::AddBSphereWithOffset( const BSphere & sphere, float offsetAsLength )
{
	float augmentedRadius = sphere.radius + offsetAsLength;
	if( augmentedRadius > radius )
	{
		radius = augmentedRadius;
	}
}

inline
void BSphere::AddPoint( V2< float > & point )
{
	float distanceFromCenter = point.Length();
	if( distanceFromCenter > radius )
	{
		radius = distanceFromCenter;
	}
}

inline
void BSphere::AddPoint( V3< float > & point )
{
	float distanceFromCenter = point.Length();
	if( distanceFromCenter > radius )
	{
		radius = distanceFromCenter;
	}
}

inline
void BSphere::AddPoint( V4< float > & point )
{
	float distanceFromCenter = point.Length();
	if( distanceFromCenter > radius )
	{
		radius = distanceFromCenter;
	}
}

