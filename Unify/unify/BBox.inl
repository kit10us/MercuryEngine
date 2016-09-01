/* Copyright (c) 2002 - 2014, Quentin S. Smith
 * All Rights Reserved
 */

template< typename T >
BBox< T >::BBox()
	: inf { V3< T >::V3Zero() }
	, sup { V3< T >::V3Zero() }
{
}

template< typename T >
BBox< T >::BBox( const V3< T > & inInf, const V3< T > & inSup )
{
	Initialize( inInf, inSup );
}

template< typename T >
BBox< T > BBox< T >::operator * ( const V3< T > & multiplicand ) const
{
	return BBox< T >( inf * multiplicand, sup * multiplicand );
}

template< typename T >
BBox< T > & BBox< T >::operator *= ( const V3< T > & multiplicand )
{
	inf *= multiplicand;
	sup *= multiplicand;
	return *this;
}

template< typename T >
BBox< T > BBox< T >::operator + ( const V3< T > & v3 ) const
{
	return BBox< T >( this->inf + v3, this->sup + v3 );
}

template< typename T >
BBox< T > & BBox< T >::operator += ( const V3< T > & v3 )
{
	inf += v3;
	sup += v3;
	return *this;
}

template< typename T >
BBox< T > BBox< T >::operator - ( const V3< T > & v3 ) const
{
	return BBox< T >( this->inf - v3, this->sup - v3 );
}

template< typename T >
BBox< T > & BBox< T >::operator -= ( const V3< T > & v3 )
{
	inf -= v3;
	sup -= v3;
	return *this;
}

// Generate the 8 vertices that make up the bounding box...
template< typename T >
void BBox< T >::GenerateCorners( V3< T > * bounds )
{
	bounds[ 0 ] = V3< T >( inf.x, inf.y, inf.z );
	bounds[ 1 ] = V3< T >( sup.x, inf.y, inf.z );
	bounds[ 2 ] = V3< T >( inf.x, sup.y, inf.z );
	bounds[ 3 ] = V3< T >( sup.x, sup.y, inf.z );
	bounds[ 4 ] = V3< T >( inf.x, inf.y, sup.z );
	bounds[ 5 ] = V3< T >( sup.x, inf.y, sup.z );
	bounds[ 6 ] = V3< T >( inf.x, sup.y, sup.z );
	bounds[ 7 ] = V3< T >( sup.x, sup.y, sup.z );
}

template< typename T >
void BBox< T >::Initialize( const V3< T > & _inf, const V3< T > & _sup )
{
	inf = _inf;
	sup = _sup;
	Fix();
}

template< typename T >
void BBox< T >::AddPoint( const V3< T > & point )
{
	if( point.x > sup.x ) sup.x = point.x;
	else if( point.x < inf.x ) inf.x = point.x;

	if( point.y > sup.y ) sup.y = point.y;
	else if( point.y < inf.y ) inf.y = point.y;

	if( point.z > sup.z ) sup.z = point.z;
	else if( point.z < inf.z ) inf.z = point.z;
}

template< typename T >
bool BBox< T >::ContainsPoint( const V3< T > & point )
{
	if( ( point.x <= sup.x && point.x >= inf.x ) && ( point.y <= sup.y && point.y >= inf.y ) && ( point.z <= sup.z && point.z >= inf.z ) )
	{
		return TRUE;
	}

	return FALSE;
}

template< typename T >
BBox< T > & BBox< T >::AddBBox( const BBox< T > & boundingBox )
{
	if( boundingBox.sup.x > sup.x ) sup.x = boundingBox.sup.x;
	if( boundingBox.inf.x < inf.x ) inf.x = boundingBox.inf.x;

	if( boundingBox.sup.y > sup.y ) sup.y = boundingBox.sup.y;
	if( boundingBox.inf.y < inf.y ) inf.y = boundingBox.inf.y;

	if( boundingBox.sup.z > sup.z ) sup.z = boundingBox.sup.z;
	if( boundingBox.inf.z < inf.z ) inf.z = boundingBox.inf.z;

	return *this;
}

template< typename T >
BBox< T > & BBox< T >::AddBBoxWithPosition( const BBox< T > & boundingBox, const V3< T > & position )
{
	// Translate bounding box.
	BBox< float > translatedBBox( boundingBox );
	translatedBBox.sup += position;
	translatedBBox.inf += position;
	return AddBBox( translatedBBox );
}

template< typename T >
const V3< T > BBox< T >::Size() const
{
	return V3< T >( sup - inf );
}

template< typename T >
void BBox< T >::Fix()
{
	if( sup.x < inf.x )
	{
		std::swap( sup.x, inf.x );
	}
	if( sup.y < inf.y )
	{
		std::swap( sup.y, inf.y );
	}
	if( sup.z < inf.z )
	{
		std::swap( sup.z, inf.z );
	}
}

template< typename T >
bool BBox< T >::RayTest( const Ray< T > & ray, T t0, T t1 ) const
{
	T tmin, tmax, tymin, tymax, tzmin, tzmax;
	V3< T > invDirection = ray.InvDirection();
	V3< int > raySign = ray.Sign();

	tmin = ( (raySign.x == 0 ? inf.x : sup.x) - ray.origin.x) * invDirection.x;
	tmax = ( (raySign.x != 0 ? inf.x : sup.x) - ray.origin.x) * invDirection.x;
	tymin = ( (raySign.y == 0 ? inf.y : sup.y) - ray.origin.y) * invDirection.y;
	tymax = ( (raySign.y != 0 ? inf.y : sup.y) - ray.origin.y) * invDirection.y;
	if( (tmin > tymax) || (tymin > tmax) )
	{
		return false;
	}
	if( tymin > tmin )
	{
		tmin = tymin;
	}
	if( tymax < tmax )
	{
		tmax = tymax;
	}
	tzmin = ( (raySign.z == 0 ? inf.z : sup.z) - ray.origin.z) * invDirection.z;
	tzmax = ( (raySign.z != 0 ? inf.z : sup.z) - ray.origin.z) * invDirection.z;
	if( (tmin > tzmax) || (tzmin > tmax) )
	{
		return false;
	}
	if( tzmin > tmin )
	{
		tmin = tzmin;
	}
	if( tzmax < tmax )
	{
		tmax = tzmax;
	}
	return ((tmin < t1) && (tmax > t0));
}

template< typename T >
bool BBox< T >::RayTest( const Ray< T > & ray, V3< T > & hitPoint ) const
{
    return false;
}

template< typename T >
V3< T > BBox< T >::ToBarrycentric( const V3< T > & point ) const
{
	V3< T > size = Size();
	V3< T > result( point - inf );
	return V3< T >( size.x != 0 ? ( result.x / size.x ) : 0, size.y != 0 ? (result.y / size.y) : 0, size.z != 0 ? (result.z / size.z) : 0 );
}

template< typename  T >
BSphere BBox< T >::MakeBSphere() const
{
	BSphere bsphere;
	bsphere.AddPoint( sup );
	bsphere.AddPoint( inf );
	return bsphere;
}