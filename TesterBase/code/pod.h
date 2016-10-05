#pragma once

#define _CRT_SECURE_NO_WARNINGS // Here to prevent the very stupid warning: warning C4996: 'WHAT EVER': This function or variable may be unsafe. Consider using strcpy_s instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.

#include <math.h>
#include <string> // We consider std::string our POD string option (well, more, our standard string implementation).

struct Origin
{
	enum TYPE
	{
		UL,
		UR,
		DL,
		DR,
		Top,
		Right,
		Bottom,
		Left,
		Center
	};
};

struct V2
{
	V2()
	{
	}

	V2( float x, float y )
		: x( x ), y( y )
	{
	}

	V2 & operator+=( const V2 in )
	{
		x += in.x;
		y += in.y;
		return *this;
	}

	V2 operator+( const V2 in ) const
	{
		V2 out( *this );
		out += in;
		return out;
	}

	V2 & operator-=( const V2 in )
	{
		x -= in.x;
		y -= in.y;
		return *this;
	}

	V2 operator-( const V2 in ) const
	{
		V2 out( *this );
		out -= in;
		return out;
	}

	V2 & operator *=( float scalar )
	{
		x *= scalar;
		y *= scalar;
		return *this;
	}

	V2 operator *( float scalar ) const
	{
		V2 out( *this );
		out *= scalar;
		return out;
	}

	float Magnitude() const
	{
		return sqrt( x * x + y * y );
	}

	float DotProduct( const V2 pointB ) const
	{
		return x * pointB.x + y * pointB.y;
	}

	float AngleInRadians( const V2 pointB ) const
	{
		V2 direction = pointB - *this;
		float angle = atan2( direction.y, direction.x );
		return angle;
	}

	float x;
	float y;
};

struct Size
{
	Size()
	{
	}

	Size( float width, float height )
		: width( width ), height( height )
	{
	}

	Size & operator+=( const Size in )
	{
		width += in.width;
		height += in.height;
		return *this;
	}

	Size operator+( const Size in ) const
	{
		Size out( *this );
		out += in;
		return out;
	}

	Size & operator-=( const Size in )
	{
		width -= in.width;
		height -= in.height;
		return *this;
	}

	Size operator-( const Size in ) const
	{
		Size out( *this );
		out -= in;
		return out;
	}

	float width;
	float height;
};

struct Rect
{
	Rect()
	{
	}

    Rect( const Rect & rect )
		: ul( rect.ul ), dr( rect.dr )
    {
    }

	Rect( V2 ul, V2 dr )
		: ul( ul ), dr( dr )
	{
	}

	Rect( float ulx, float uly, float drx, float dry )
		: ul( ulx, uly ), dr( drx, dry )
	{
	}

	Size GetSize() const
	{
		return Size( dr.x - ul.x, dr.y - ul.y );
	}

	V2 Center() const
	{
		Size sizeHalf( GetSize().width * 0.5f, GetSize().height * 0.5f ); 
		return V2( ul.x + sizeHalf.width, ul.y + sizeHalf.height );
	}

	bool In( V2 position ) const
	{
		if (  position.x >= ul.x && position.y >= ul.y && position.x <= dr.x && position.y <= dr.y )
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	Rect & operator+=( const V2 v )
	{
		ul += v;
		dr += v;
		return *this;
	}

	Rect operator+( const V2 v )
	{
		Rect rect( *this );
		rect.ul += v;
		rect.dr += v;
		return rect;
	}

	V2 ul;
	V2 dr;
};