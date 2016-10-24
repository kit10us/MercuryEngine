// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#include <unify/String.h>
#include <unify/Color.h>
#include <unify/ColorUnit.h>
#include <algorithm>

using namespace unify;

ColorUnit ColorUnit::ColorUnitRGBA( float r, float g, float b, float a )
{
	return ColorUnit( r, g, b, a );
}

ColorUnit ColorUnit::ColorUnitARGB( float a, float r, float g, float b )
{
	return ColorUnit::ColorUnitRGBA( r, g, b, a );
}

ColorUnit ColorUnit::ColorUnitRGB( float r, float g, float b )
{
	return ColorUnit::ColorUnitRGBA( r, g, b, 1.0f );
}

ColorUnit ColorUnit::ColorUnitWhite( float a )
{
	return ColorUnit::ColorUnitRGBA( 1.0f, 1.0f, 1.0f, a );
}

ColorUnit ColorUnit::ColorUnitRed( float r, float a )
{
	return ColorUnit::ColorUnitRGBA( r, 0.0f, 0.0f, a );
}

ColorUnit ColorUnit::ColorUnitGreen( float g, float a )
{
	return ColorUnit::ColorUnitRGBA( 0.0f, g, 0.0f, a );
}

ColorUnit ColorUnit::ColorUnitBlue( float b, float a )
{
	return ColorUnit::ColorUnitRGBA( 0.0f, 0.0f, b, a );
}

ColorUnit ColorUnit::ColorUnitYellow( float b, float a )
{
	return ColorUnit::ColorUnitRGBA( b, b, 0.0f, a );
}

ColorUnit ColorUnit::ColorUnitCyan( float b, float a )
{
	return ColorUnit::ColorUnitRGBA( b, b, 0.0f, a );
}

ColorUnit ColorUnit::ColorUnitMagenta( float b, float a )
{
	return ColorUnit::ColorUnitRGBA( b, 0.0f, b, a );
}

ColorUnit ColorUnit::ColorUnitGrey( float grey, float a )
{
	return ColorUnit::ColorUnitRGBA( grey, grey, grey, a );
}

ColorUnit ColorUnit::ColorUnitBlack( float a )
{
	return ColorUnit::ColorUnitRGBA( 0, 0, 0, a );
}

ColorUnit ColorUnit::ColorUnitZero()
{
	return ColorUnit::ColorUnitRGBA( 0, 0, 0, 0 );
}

ColorUnit ColorUnit::ColorUnitLerp( ColorUnit l, ColorUnit r, float ratio )
{
	return l * (1.0f - ratio) + r * ratio;
}																  

ColorUnit::ColorUnit()
{
}


ColorUnit::ColorUnit( float r, float g, float b, float a )
: a( a )
, r( r )
, g( g )
, b( b )
{
}


ColorUnit::ColorUnit( const ColorUnit & color )
: a( color.a )
, r( color.r )
, g( color.g )
, b( color.b )
{
}

ColorUnit::ColorUnit( const std::string &text, Order order, float defaultAlpha )
{
	r = 0;
	g = 0;
	b = 0;
	a = defaultAlpha;

	if( StringIs( text, "white" ) )
	{
		r = 1.0f;
		g = 1.0f;
		b = 1.0f;
	}
	else if( StringIs( text, "black" ) )
	{
		// Nothing (use the above defaults).
	}
	else if( StringIs( text, "red" ) )
	{
		r = 1.0f;
	}
	else if( StringIs( text, "green" ) )
	{
		g = 1.0f;
	}
	else if( StringIs( text, "blue" ) )
	{
		b = 1.0f;
	}
	else if( StringIs( text, "clear" ) )
	{
		a = 0.0f;
	}
	else
	{
		std::vector< float > split = unify::Split< float >( text, ',' );
		if ( split.size() == 1 )
		{
			split = unify::SplitOnWhitespace< float >( text );
		}

		if ( split.size() == 3 )
		{
			r = split[ 0 ];
			g = split[ 1 ];
			b = split[ 2 ];
		}
		else if ( split.size() == 4 )
		{
			switch( order )
			{
			case RGBA:
				r = split[ 0 ];
				g = split[ 1 ];
				b = split[ 2 ];
				a = split[ 3 ];
				break;
			case ARGB:
				a = split[ 0 ];
				r = split[ 1 ];
				g = split[ 2 ];
				b = split[ 3 ];
				break;
			case BGRA:
				b = split[ 0 ];
				g = split[ 1 ];
				r = split[ 2 ];
				a = split[ 3 ];
				break;
			case ABGR:
				a = split[ 0 ];
				b = split[ 1 ];
				g = split[ 2 ];
				r = split[ 3 ];
				break;
			}
		}
	}
}

ColorUnit::operator Color()
{
	return Color::ColorRGBA( (unsigned char)(r * 255.0f), (unsigned char)(g * 255.0f), (unsigned char)(b * 255.0f), (unsigned char)(a * 255.0f) );
}

float ColorUnit::SumComponents()
{
	return a + r + g + b;
}

ColorUnit & ColorUnit::operator += ( const ColorUnit & col )
{
	a += col.a;
	r += col.r;
	g += col.g;
	b += col.b;
	return *this;
}


ColorUnit & ColorUnit::operator -= ( const  ColorUnit & col )
{
	a -= col.a;
	r -= col.r;
	g -= col.g;
	b -= col.b;
	return *this;
}


ColorUnit & ColorUnit::operator *= ( float val )
{
	a *= val;
	r *= val;
	g *= val;
	b *= val;
	return *this;
}


ColorUnit & ColorUnit::operator /= ( float val )
{
	a /= val;
	r /= val;
	g /= val;
	b /= val;
	return *this;
}

// binary

ColorUnit ColorUnit::operator + ( const ColorUnit & col ) const
{
	return ColorUnit::ColorUnitARGB( a + col.a, r + col.r, g + col.g, b + col.b );
}


ColorUnit ColorUnit::operator - ( const ColorUnit & col ) const
{
	return ColorUnit::ColorUnitARGB( a - col.a, r - col.r, g - col.g, b - col.b );
}


ColorUnit ColorUnit::operator * ( const ColorUnit & col ) const
{
	return ColorUnit::ColorUnitARGB( a * col.a, r * col.r, g * col.g, b * col.b );
}

ColorUnit ColorUnit::operator / ( const ColorUnit & col ) const
{
	return ColorUnit::ColorUnitARGB( a / col.a, r / col.r, g / col.g, b / col.b );
}


ColorUnit ColorUnit::operator * ( float val ) const
{
	return ColorUnit::ColorUnitARGB( a * val, r * val, g * val, b * val );
}


ColorUnit ColorUnit::operator / ( float val ) const
{
	return ColorUnit::ColorUnitARGB( a / val, r / val, g / val, b / val );
}

bool ColorUnit::operator == ( const ColorUnit & col ) const
{
	return( a == col.a && r == col.r && g == col.g && b == col.b );
}


bool ColorUnit::operator != ( const ColorUnit & col ) const
{
	return( a != col.a || r != col.r || g != col.g || b != col.b );
}

void ColorUnit::SetRed( float pr )
{
	r = pr;
}

void ColorUnit::SetGreen( float pg )
{
	g = pg;
}

void ColorUnit::SetBlue( float pb )
{
	b = pb;
}

void ColorUnit::SetAlpha( float pa )
{
	a = pa;
}

void ColorUnit::SetRGBA( float ri, float gi, float bi, float ai )
{
	r = ri;
	g = gi;
	b = bi;
	a = ai;
}

ColorUnit ColorUnit::Normalize() const
{
	ColorUnit result = ColorUnitRGBA(
		std::min< float >( std::max< float >( r, 1.0f ), 0.0f ),
		std::min< float >( std::max< float >( r, 1.0f ), 0.0f ),
		std::min< float >( std::max< float >( r, 1.0f ), 0.0f ),
		std::min< float >( std::max< float >( r, 1.0f ), 0.0f ) 
	);
	return result;
}

std::string ColorUnit::ToString( ColorUnit::Order order ) const
{
	switch( order )
	{
	case RGBA:
	default:
		return Cast< std::string >( r ) + ", " + Cast< std::string >( g ) + ", " + Cast< std::string >( b ) + ", " + Cast< std::string >( a );
	case ARGB:
		return Cast< std::string >( a ) + ", " + Cast< std::string >( r ) + ", " + Cast< std::string >( g ) + ", " + Cast< std::string >( b );
	case BGRA:
		return Cast< std::string >( b ) + ", " + Cast< std::string >( g ) + ", " + Cast< std::string >( r ) + ", " + Cast< std::string >( a );
	case ABGR:
		return Cast< std::string >( a ) + ", " + Cast< std::string >( b ) + ", " + Cast< std::string >( g ) + ", " + Cast< std::string >( r );
	}
}
