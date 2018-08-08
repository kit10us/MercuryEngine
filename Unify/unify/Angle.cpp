// Copyright (c) 2002 - 2019, Evil Quail LLC
// All Rights Reserved

#include <unify/Angle.h>
#include <unify/Cast.h>
#include <cmath>

using namespace unify;

const float DTOR = (3.14159265f / 180.0f);

Angle unify::AngleInRadians( float radians )
{
	return Angle( radians );
}

Angle unify::AngleInDegrees( float degrees )
{
	return Angle( degrees * DTOR );
}

Angle unify::AnglePI()
{
	return AngleInRadians( 3.14159265f );
}

Angle unify::AnglePI2()
{
	return AngleInRadians( 3.14159265f * 2.0f );
}

Angle unify::AnglePIHalf()
{
	return AngleInRadians( 3.14159265f * 0.5f );
}


Angle::Angle( float radians )
: m_radians( radians )
{
}

Angle::Angle( std::string angle )
{
	char last = angle.back();
	if ( last == 'd' || last == 'D' )
	{
		angle.pop_back();
		m_radians = (float)atof( angle.c_str() ) * DTOR;
	}
	else if ( last == 'r' || last == 'R' )
	{
		angle.pop_back();
		m_radians = (float)atof( angle.c_str() );
	}
}

Angle::Angle()
: m_radians( 0.0f )
{
}

Angle::Angle( const Angle & angle )
	: m_radians( angle.m_radians )
{
}

Angle & Angle::operator = ( const Angle & angle )
{
    m_radians = angle.m_radians;
    return *this;
}

Angle & Angle::operator += ( const Angle & angle )
{
    m_radians += angle.m_radians;
    return *this;
}

Angle & Angle::operator -= ( const Angle & angle )
{
    m_radians -= angle.m_radians;
    return *this;
}

Angle & Angle::operator *= ( const Angle & angle )
{
    m_radians *= angle.m_radians;
    return *this;
}

Angle & Angle::operator /= ( const Angle & angle )
{
    m_radians /= angle.m_radians;
    return *this;
}

Angle Angle::operator + ( const Angle & angle) const
{
    Angle returnAngle( *this );
    returnAngle += angle;
    return returnAngle;
}

Angle Angle::operator - ( const Angle & angle ) const
{
    Angle returnAngle( *this );
    returnAngle -= angle;
    return returnAngle;
}

Angle Angle::operator * ( const Angle & angle ) const
{
    Angle returnAngle( *this );
    returnAngle *= angle;
    return returnAngle;
}

Angle Angle::operator / ( const Angle & angle ) const
{
    Angle returnAngle( *this );
    returnAngle /= angle;
    return returnAngle;
}

bool Angle::operator == ( const Angle & angle ) const
{
    return m_radians == angle.m_radians;
}

Angle & Angle::operator *= ( float scalar )
{
    m_radians *= scalar;
    return *this;
}

Angle & Angle::operator /= ( float scalar )
{
    m_radians /= scalar;
    return *this;
}

Angle Angle::operator * ( float scalar ) const
{
    Angle returnAngle( *this );
    returnAngle.m_radians *= scalar;
    return returnAngle;
}

Angle Angle::operator / ( float scalar ) const
{
    Angle returnAngle( *this );
    returnAngle.m_radians /= scalar;
    return returnAngle;
}

bool Angle::operator > ( const Angle & angle )
{
	return m_radians > angle.m_radians;
}

bool Angle::operator >= ( const Angle & angle )
{
	return m_radians >= angle.m_radians;
}

bool Angle::operator < ( const Angle & angle )
{
	return m_radians < angle.m_radians;
}

bool Angle::operator <= ( const Angle & angle )
{
	return m_radians <= angle.m_radians;
}

float Angle::ToRadians() const
{
    return m_radians;
}

float Angle::ToDegrees() const
{
    return m_radians * (180.0f / 3.14159265f);
}

int Angle::Fix360()
{
    const float pi2 = (3.14159265f * 2.0f);
	int magnitude = 0;
    while( m_radians > pi2 )
    {
        m_radians -= pi2;
		magnitude -= 1;
    }
    while( m_radians < 0 )
    {
        m_radians += pi2;
		magnitude += 1;
    }
	return magnitude;
}

float Angle::SinOf() const
{
	return (float)sin( m_radians );
}

float Angle::CosOf() const
{
	return (float)cos( m_radians );
}

std::string Angle::ToString( bool radians ) const
{
	std::string out;
	if ( radians )
	{
		out = unify::Cast< std::string >( ToRadians() );
		out.push_back( 'r' );
	}
	else
	{
		out = unify::Cast< std::string >( ToDegrees() );
		out.push_back( 'd' );
	}
	return out;
}
