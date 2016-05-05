#include "DXI/extutypes.h"

///////////////////////////////
// TV2
///////////////////////////////

template< class T >
dxi::TV2<T>::TV2()
{
}

template< class T >
dxi::TV2<T>::TV2( T x, T y )
{
	this->x = x;
	this->y = y;
}

template< class T >
dxi::TV2<T>::TV2( const TV2< T > & vec )
{
	x = vec.x;
	y = vec.y;
}


////////////////////////////////
// TV3
////////////////////////////////

dxi::TV3::TV3()
{
}

dxi::TV3::TV3( qlib::TFloat x, qlib::TFloat y, qlib::TFloat z )
{
	this->x = x;
	this->y = y;
	this->z = z;
}


//////////////////////////////////////////////
// TV4
//////////////////////////////////////////////

dxi::TV4::TV4()
{
}

dxi::TV4::TV4( qlib::TFloat x, qlib::TFloat y, qlib::TFloat z, qlib::TFloat w )
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}


///////////////////////////////
// TSize
///////////////////////////////

template< class T >
dxi::TSize<T>::TSize()
{
}

template< class T >
dxi::TSize<T>::TSize( T width, T height )
{
	this->width = width;
	this->height = height;
}

template< class T >
dxi::TSize<T>::TSize( const dxi::TSize< T > & size )
{
	width = size.width;
	height = size.height;
}


///////////////////////////////
// TColor
///////////////////////////////

// Specialized dxi::TColors...
dxi::TColor dxi::TColor::RGBA( qlib::TUInt8 r, qlib::TUInt8 g, qlib::TUInt8 b, qlib::TUInt8 a )
{
	return dxi::TColor( r, g, b, a );
}

dxi::TColor dxi::TColor::ARGB( qlib::TUInt8 a, qlib::TUInt8 r, qlib::TUInt8 g, qlib::TUInt8 b )
{
	return dxi::TColor( r, g, b, a );
}

dxi::TColor dxi::TColor::RGB( qlib::TUInt8 r, qlib::TUInt8 g, qlib::TUInt8 b )
{
	return dxi::TColor( r, g, b, 255 );
}

dxi::TColor dxi::TColor::White()
{
	return dxi::TColor( 255, 255, 255, 255 );
}

dxi::TColor dxi::TColor::Red( qlib::TUInt8 r )
{
	return dxi::TColor( r, 255, 255, 255 );
}

dxi::TColor dxi::TColor::Green( qlib::TUInt8 g )
{
	return dxi::TColor( 255, g, 255, 255 );
}

dxi::TColor dxi::TColor::Blue( qlib::TUInt8 b )
{
	return dxi::TColor( 255, 255, b, 255 );
}

dxi::TColor dxi::TColor::Grey( qlib::TUInt8 grey )
{
	return dxi::TColor( grey, grey, grey, 255 );
}

dxi::TColor dxi::TColor::Black()
{
	return dxi::TColor( 0, 0, 0, 255 );
}

dxi::TColor dxi::TColor::Zero()
{
	return dxi::TColor( 0, 0, 0, 0 );
}
