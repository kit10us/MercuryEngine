// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

template< typename T >
V4< T >::V4()
{
}

template< typename T >
V4< T >::V4( const V4< T > & v4 )
: x( v4.x )
, y( v4.y )
, z( v4.z )
, w( v4.w )
{
}

template< typename T >
V4< T >::V4( T x, T y, T z, T w )
: x( x )
, y( y )
, z( z )
, w( w )
{
}

template< typename T >
V4< T >::V4( const T arr[ 4 ] )
: x( arr[ 0 ] )
, y( arr[ 1 ] )
, z( arr[ 2 ] )
, w( arr[ 3 ] )
{
}

template< typename T >
V4< T >::V4( const V3< T > & v3, float w )
: x( v3.x )
, y( v3.y )
, z( v3.z )
, w( w )
{
}

template< typename T >
V4< T >::V4( const std::string & text )
{
	std::vector< T > split = Split< T >( text, ',' );
    x = split[ 0 ];
    y = split[ 1 ];
    z = split[ 2 ];
    w = split[ 3 ];
}

template< typename T >
V4< T > & V4< T >::operator += ( const V4< T >& vec )
{
	x += vec.x;
	y += vec.y;
	z += vec.z;
	w += vec.w;
	return *this;
}

template< typename T >
V4< T > & V4< T >::operator -= ( const  V4< T >& vec )
{
	x -= vec.x;
	y -= vec.y;
	z -= vec.z;
	w -= vec.w;
	return *this;
}

template< typename T >
V4< T > & V4< T >::operator *= ( const V4< T >& vec )
{
	x *= vec.x;
	y *= vec.y;
	z *= vec.z;
	w *= vec.w;
	return *this;
}

template< typename T >
V4< T > & V4< T >::operator /= ( const V4< T >& vec )
{
	x /= vec.x;
	y /= vec.y;
	z /= vec.z;
	w /= vec.w;
	return *this;
}

template< typename T >
V4< T > & V4< T >::operator *= ( T val )
{
	x *= val;
	y *= val;
	z *= val;
	w *= val;
	return *this;
}

template< typename T >
V4< T > & V4< T >::operator /= ( T val )
{
	x /= val;
	y /= val;
	z /= val;
	w /= val;
	return *this;
}

// binary
template< typename T >
V4< T > V4< T >::operator + ( const V4< T > & vec) const
{
	return V4( x + vec.x, y + vec.y, z + vec.z, w + vec.w );
}

template< typename T >
V4< T > V4< T >::operator - ( const V4< T > & vec ) const
{
	return V4( x - vec.x, y - vec.y, z - vec.z, w - vec.w );
}

template< typename T >
V4< T > V4< T >::operator * ( const V4< T > & vec ) const
{
	return V4( x * vec.x, y * vec.y, z * vec.z, w * vec.w );
}

template< typename T >
V4< T > V4< T >::operator / ( const V4< T > & vec ) const
{
	return V4< T >( x / vec.x, y / vec.y, z / vec.z, w / vec.w );
}

template< typename T >
V4< T > V4< T >::operator * ( T val ) const
{
	return V4( x * val, y * val, z * val, z * val );
}

template< typename T >
V4< T > V4< T >::operator / ( T val ) const
{
	return V4( x / val, y / val, z / val, w / val );
}

template< typename T >
bool V4< T >::operator == ( const V4< T > & vec ) const
{
	return( x == vec.x && y == vec.y && z == vec.z && w == vec.w );
}

template< typename T >
bool V4< T >::operator != ( const V4< T > & vec ) const
{
	return( x != vec.x || y != vec.y || z != vec.z || w != vec.w );
}

template< typename T >
T & V4< T >::operator[]( size_t i )
{
    switch( i )
    {
    case 0:
        return x;
    case 1:
        return y;
    case 2:
        return z;
    case 3:
        return w;
    default:
        throw unify::Exception( "Out of range!" );
    }
}

template< typename T >
const T & V4< T >::operator[]( size_t i ) const
{
    switch( i )
    {
    case 0:
        return x;
    case 1:
        return y;
    case 2:
        return z;
    case 3:
        return w;
    default:
        throw unify::Exception( "Out of range!" );
    }
}


template< typename T >
void V4< T >::Zero()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	w = 0.0f;
}

template< typename T >
T V4< T >::Length() const
{
	return sqrt( (x * x ) + (y * y) + (z * z) + (w * w) );
	//return D3DXVec4Length( (D3DXVECTOR4*)this );
}

template< typename T >
void V4< T >::Normalize()
{
	float length = Length();
	x /= length;
	y /= length;
	z /= length;
	w /= length;
	//qV4Normalize( this, this );
}

/*
template< typename T >
void V4< T >::Transform( const TMatrix* pMat )
{
	qV4Transform( this, this, pMat );
}
*/

template< typename T >
std::string V4< T >::ToString() const
{
    return Cast< std::string >( x ) + ", " + Cast< std::string >( y ) + ", " + Cast< std::string >( z ) + ", " + Cast< std::string >( w );
}
